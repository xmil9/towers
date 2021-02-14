//
// Feb-2021, Michael Lindner
// MIT license
//
#include "towers_game.h"
#include "animation_factory.h"
#include "animation_tags.h"
#include "attacker_models.h"
#include "defender_models.h"
#include "map_data.h"
#include "texture_tags.h"
// External
#include "opengl_util/gll_debug.h"
// Runtime
#include <iostream>
#include <thread>


namespace
{

///////////////////

constexpr gfl::Lib::ContextVersion OpenGLVersion{4, 6};
constexpr float MouseSensitivity = 0.05f;
constexpr float ScrollSensitivity = 2.0f;
constexpr float MovementSpeed = 2.5f;


///////////////////

static sge::MapPos truncate(sge::MapPos pos)
{
   return sge::MapPos(static_cast<int>(pos.x), static_cast<int>(pos.y));
}


static sge::MapPos centerInField(sge::MapPos pos)
{
   return truncate(pos) + sge::MapVec{.5f, .5f};
}

} // namespace


///////////////////

Towers::Towers()
: Game2{WndWidth, WndHeight, "towsers"}, m_glfw{OpenGLVersion, GLFW_OPENGL_CORE_PROFILE},
  m_dashboard{DashboardWidth, DashboardHeight, this, this}
{
}


bool Towers::setup()
{
   return (setupUi() && setupInput() && setupOutput() && setupTextures() &&
           setupTerrain() && setupRenderer() && setupAnimations() && setupAttackers() &&
           setupDefenders() && setupBackground() &&
           m_dashboard.setup(m_renderer, m_coordSys.get()));
}


void Towers::cleanup()
{
}


void Towers::run()
{
   m_frameClock.nextLap();
   float processLag = 0.f;

   while (!m_mainWnd.shouldClose())
   {
      m_frameClock.nextLap();
      if (!m_isPaused)
         processLag += m_frameClock.lapLength();

      processInput();

      if (!m_isPaused)
      {
         // Catch up with the game state for the time that was used for rendering and
         // processing input. This standardizes the game speed across machines with
         // various processing speeds.
         constexpr float UpdateGranularityMs = 25.f;
         while (processLag >= UpdateGranularityMs)
         {
            updateState();
            processLag -= UpdateGranularityMs;
         }
      }

      render();
   }
}


bool Towers::setupUi()
{
   if (m_glfw.init(gfl::Lib::DebugOuput::On) != GLFW_NO_ERROR)
      return false;

   if (!setupMainWindow())
      return false;

   glfwMakeContextCurrent(m_mainWnd.handle());

   if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
      return false;

   return true;
}


bool Towers::setupMainWindow()
{
   m_mainWnd.setInputController(&m_input);
   m_mainWnd.addObserver([this](sge::MainWindow& src, std::string_view event,
                                const esl::ObservedEventData& data) {
      onMainWindowChanged(src, event, data);
   });

   if (m_mainWnd.create(WndWidth, WndHeight, "towers") != GLFW_NO_ERROR)
      return false;

   // Hide mouse cursor and capture its events even outside the main window.
   // m_mainWnd.setCursorMode(GLFW_CURSOR_DISABLED);
   return true;
}


bool Towers::setupInput()
{
   m_input.addObserver([this](sge::Input& /*src*/, std::string_view event,
                              const esl::ObservedEventData& data) {
      onInputChanged(m_input, event, data);
   });
   return true;
}


bool Towers::setupOutput()
{
   if (gll::haveDebugContext())
      gll::setDebugOutputCallback();
   return true;
}


bool Towers::setupTextures()
{
   const std::filesystem::path scene = m_paths.scenePath();
   const std::filesystem::path ui = m_paths.uiPath();

   struct TextureSpec
   {
      std::string tag;
      const std::filesystem::path& path;
      std::filesystem::path filename;
   };

   const std::vector<TextureSpec> textures{
      {AatTexture, scene, "aat.png"},
      {AatHitTexture, scene, "aat_hit.png"},
      {MhcTexture, scene, "mhc.png"},
      {MhcHitTexture, scene, "mhc_hit.png"},
      {LtTexture, scene, "lt.png"},
      {LtFiring1Texture, scene, "lt_firing1.png"},
      {LtFiring2Texture, scene, "lt_firing2.png"},
      {SmTexture, scene, "sm.png"},
      {SmFiring1Texture, scene, "sm_firing1.png"},
      {SmFiring2Texture, scene, "sm_firing2.png"},
      {Explosion1TTag, scene, "explosion1.png"},
      {Explosion2TTag, scene, "explosion2.png"},
      {Explosion3TTag, scene, "explosion3.png"},
      {Explosion4TTag, scene, "explosion4.png"},
      {Map1TTag, scene, "map1.png"},
      {InvalidFieldTTag, scene, "invalid_field.png"},
      {RangeTTag, scene, "range.png"},
      {HpStatusTTag, scene, "hp_status.png"},
      {DashboardTTag, ui, "dashboard.png"},
      {ButtonBackgroundTTag, ui, "defender_button_bgrd.png"},
      {StartTTag, ui, "start.png"},
      {PauseTTag, ui, "pause.png"},
   };

   for (const auto& spec : textures)
      if (!m_resources.loadTexture(spec.tag, spec.path / spec.filename))
         return false;
   return true;
}


bool Towers::setupTerrain()
{
   std::optional<MapData> mapData = loadMapData(m_paths.mapPath() / "map.json");
   if (!mapData)
      return false;
   m_map = std::make_unique<Map>(std::move(*mapData));

   const sge::PixDim mapPixDim{MapWidth, MapHeight};
   const sge::IntDim mapSizeInFields = m_map->sizeInFields();
   const sge::PixDim fieldPixDim{mapPixDim.x / mapSizeInFields.x,
                                 mapPixDim.y / mapSizeInFields.y};
   m_coordSys = std::make_unique<MapCoordSys>(fieldPixDim);

   return true;
}


bool Towers::setupRenderer()
{
   constexpr sge::NormDim hpStatusDim{.5f, .05f};
   const sge::PixDim hpStatusPixDim = toPix(hpStatusDim);
   constexpr sge::NormVec hpOffset{-.25f, -.4f};
   const sge::PixVec hpPixOffset = toPix(hpOffset);
   m_hpRenderer = std::make_unique<HpRenderer>(
      sge::Sprite{sge::SpriteLook{HpStatusTTag}, sge::SpriteForm{hpStatusPixDim}},
      hpPixOffset);

   return m_renderer.setup(&m_resources, m_paths.shaderPath(), m_paths.fontPath(),
                           WndWidth, WndHeight);
}


bool Towers::setupAnimations()
{
   assert(!!m_coordSys);

   AnimationFactory factory;

   m_resources.addAnimation(ExplosionATag,
                            factory.make(ExplosionATag, toPix(sge::MapDim{1.f, 1.f})));

   sge::PixDim firingSize =
      toPix(scaleInto(toMap(m_resources.getTextureSize(LtTexture)), LtSize));
   m_resources.addAnimation(LtFiringAnimation,
                            factory.make(LtFiringAnimation, firingSize));

   firingSize = toPix(scaleInto(toMap(m_resources.getTextureSize(SmTexture)), SmSize));
   m_resources.addAnimation(SmFiringAnimation,
                            factory.make(SmFiringAnimation, firingSize));

   return true;
}


bool Towers::setupAttackers()
{
   assert(!!m_coordSys);
   assert(!!m_map);

   m_attackFactory = std::make_unique<AttackerFactory>(m_coordSys.get());

   m_attackFactory->registerModel(
      AatModel,
      AttackerLook{
         sge::Sprite{sge::SpriteLook{AatTexture},
                     sge::SpriteForm{m_resources.getTextureSize(AatTexture)}},
         sge::Sprite{sge::SpriteLook{AatHitTexture},
                     sge::SpriteForm{m_resources.getTextureSize(AatHitTexture)}},
         m_resources.getAnimation(ExplosionATag), m_hpRenderer.get()},
      AssaultTank::defaultAttributes());
   m_attackFactory->registerModel(
      MhcModel,
      AttackerLook{
         sge::Sprite{sge::SpriteLook{MhcTexture},
                     sge::SpriteForm{m_resources.getTextureSize(MhcTexture)}},
         sge::Sprite{sge::SpriteLook{MhcHitTexture},
                     sge::SpriteForm{m_resources.getTextureSize(MhcHitTexture)}},
         m_resources.getAnimation(ExplosionATag), m_hpRenderer.get()},
      MobileCannon::defaultAttributes());

   // Small initial delay to keep attackers from rendering before the attack has started.
   constexpr int DefaultDelay = 1;

   addAttacker(m_attackFactory->makeAttacker(
      AatModel, OffsetPath{&m_map->path(), sge::MapVec{0.f, 0.f}}, DefaultDelay));
   addAttacker(m_attackFactory->makeAttacker(
      AatModel, OffsetPath{&m_map->path(), sge::MapVec{0.f, 0.f}}, 30));
   addAttacker(m_attackFactory->makeAttacker(
      MhcModel, OffsetPath{&m_map->path(), sge::MapVec{-.08f, .05f}}, DefaultDelay));
   addAttacker(m_attackFactory->makeAttacker(
      MhcModel, OffsetPath{&m_map->path(), sge::MapVec{0.f, -0.05}}, DefaultDelay));

   return true;
}


bool Towers::setupDefenders()
{
   assert(!!m_coordSys);
   assert(!!m_map);

   m_defenseFactory = std::make_unique<DefenderFactory>(m_coordSys.get(), &m_attackers);

   m_defenseFactory->registerModel(
      LtModel,
      DefenderLook{sge::Sprite{sge::SpriteLook{LtTexture},
                               sge::SpriteForm{m_resources.getTextureSize(LtTexture)}},
                   m_resources.getAnimation(LtFiringAnimation)},
      LaserTurret::defaultAttributes());

   m_defenseFactory->registerModel(
      SmModel,
      DefenderLook{sge::Sprite{sge::SpriteLook{SmTexture},
                               sge::SpriteForm{m_resources.getTextureSize(SmTexture)}},
                   m_resources.getAnimation(SmFiringAnimation)},
      SonicMortar::defaultAttributes());

   resetDefenderPlacements();
   return true;
}


bool Towers::setupBackground()
{
   m_background =
      sge::Sprite{sge::SpriteLook{Map1TTag}, sge::SpriteForm{{MapWidth, MapHeight}}};

   const sge::PixDim fieldPixDim = toPix(sge::MapDim{1.f, 1.f});
   m_invalidFieldOverlay =
      sge::Sprite{sge::SpriteLook{InvalidFieldTTag}, sge::SpriteForm{fieldPixDim}};
   m_rangeOverlay = sge::Sprite{sge::SpriteLook{RangeTTag}, sge::SpriteForm{fieldPixDim}};

   return true;
}


void Towers::processInput()
{
   m_input.process(m_mainWnd, m_frameClock.lapLength(sge::MsToSecs));
}


void Towers::updateState()
{
   if (m_isPaused)
      return;

   for (auto& entry : m_attackers)
      entry.second.update();
   for (auto& defender : m_defenders)
      defender.update();

   removeDestroyedAttackers();
}


void Towers::render()
{
   m_renderer.clearScene();

   m_renderer.beginRendering();
   renderMap();
   m_dashboard.render(m_renderer, sge::PixPos{MapWidth - 1, 0.f});

   for (auto& entry : m_attackers)
      entry.second.render(m_renderer, m_isPaused);
   for (auto& defender : m_defenders)
      defender.render(m_renderer, m_isPaused);

   renderDefenderInfo();
   // Draw placed content on top of everything else.
   renderPlaceSession();

   m_mainWnd.swapBuffers();
}


void Towers::renderMap()
{
   m_renderer.renderSprite(m_background, {0.f, 0.f});
}


void Towers::renderDefenderInfo()
{
   if (m_placeSess)
      return;

   const sge::PixPos mousePixPos{m_mainWnd.mousePosition()};
   const sge::MapPos fieldPos = truncate(toMap(mousePixPos));
   if (!m_map->isOnMap(fieldPos))
      return;

   const Defender* touchedDefender = defenderOnField(fieldPos);
   if (touchedDefender)
   {
      const sge::MapCoord range = touchedDefender->range();
      const sge::PixDim rangeDim = toPix(2.0f * sge::MapDim{range, range});
      const sge::MapPos at = touchedDefender->center();
      const sge::PixPos atPix = toPix(at);
      m_rangeOverlay.setSize(rangeDim);
      m_renderer.renderSpriteCentered(m_rangeOverlay, atPix);
   }
}


void Towers::renderPlaceSession()
{
   if (m_placeSess)
   {
      const sge::PixPos mousePixPos{m_mainWnd.mousePosition()};

      const sge::MapPos fieldPos = truncate(toMap(mousePixPos));
      const sge::PixPos fieldPixPos = toPix(fieldPos);
      const sge::PixPos fieldCenterPixPos = fieldPixPos + toPix(sge::MapPos{.5f, .5f});

      const bool isOnMap = m_map->isOnMap(fieldPos);
      if (isOnMap)
      {
         if (canPlaceDefenderOnField(m_placeSess->model, fieldPos))
         {
            const sge::PixDim rangeDim =
               toPix(2.0f * sge::MapDim{m_placeSess->range, m_placeSess->range});
            m_rangeOverlay.setSize(rangeDim);
            m_renderer.renderSpriteCentered(m_rangeOverlay, fieldCenterPixPos);
         }
         else
         {
            m_renderer.renderSprite(m_invalidFieldOverlay, fieldPixPos);
         }
      }

      const sge::PixPos indicatorCenter = isOnMap ? fieldCenterPixPos : mousePixPos;
      m_renderer.renderSpriteCentered(m_placeSess->indicator, indicatorCenter);
   }
}


void Towers::addAttacker(std::optional<Attacker>&& attacker)
{
   if (attacker)
   {
      attacker->addObserver(
         [this](auto& src, std::string_view event, const esl::ObservedEventData& data) {
            onAttackerDestroyed(src, event, data);
         });

      const EntityId id = attacker->id();
      m_attackers[id] = std::move(*attacker);
   }
}


template <typename SpecificAttacker>
void Towers::onAttackerDestroyed(SpecificAttacker& src, std::string_view /*event*/,
                                 const esl::ObservedEventData& /*data*/)
{
   m_credits += src.reward();
   removeAsTarget(src.id());
}


void Towers::removeDestroyedAttackers()
{
   for (auto it = m_attackers.begin(); it != m_attackers.end();)
   {
      if (it->second.canBeRemoved())
         it = m_attackers.erase(it);
      else
         ++it;
   }
}


void Towers::removeAsTarget(EntityId attackerId)
{
   for (auto& defender : m_defenders)
      defender.removeAsTarget(attackerId);
}


void Towers::resetDefenderPlacements()
{
   assert(m_map);
   const sge::IntDim mapDim = m_map->sizeInFields();
   m_defenderMatrix.resize(mapDim.x * mapDim.y);
   std::fill(m_defenderMatrix.begin(), m_defenderMatrix.end(), false);
}


bool Towers::hasDefenderOnField(sge::MapPos field) const
{
   const int idx =
      static_cast<int>(field.y) * m_map->sizeInFields().x + static_cast<int>(field.x);
   assert(idx >= 0 && idx < m_defenderMatrix.size());
   return m_defenderMatrix[idx];
}


const Defender* Towers::defenderOnField(sge::MapPos field) const
{
   if (hasDefenderOnField(field))
      for (const auto& defender : m_defenders)
         if (m_map->isOnSameField(defender.center(), field))
            return &defender;
   return nullptr;
}


void Towers::setDefenderOnField(sge::MapPos field, bool hasDefender)
{
   const int idx =
      static_cast<int>(field.y) * m_map->sizeInFields().x + static_cast<int>(field.x);
   assert(idx >= 0 && idx < m_defenderMatrix.size());
   m_defenderMatrix[idx] = hasDefender;
}


bool Towers::canPlaceDefenderOnField(const std::string& /*defenderModel*/,
                                     sge::MapPos field) const
{
   return m_map->canBuildOnField(field) && !hasDefenderOnField(field);
}


void Towers::addDefender(std::optional<Defender>&& defender, const sge::PixPos& pos)
{
   if (defender)
   {
      m_credits -= defender->cost();
      m_defenders.push_back(*defender);
      setDefenderOnField(pos, true);
   }
}


void Towers::placeDefender(const sge::PixPos& mousePos)
{
   const sge::MapPos pos = centerInField(toMap(mousePos));
   if (!m_map->isOnMap(pos) || !canPlaceDefenderOnField(m_placeSess->model, pos))
      return;

   addDefender(m_defenseFactory->makeDefender(m_placeSess->model, pos), pos);
}


void Towers::onMainWindowChanged(sge::MainWindow& /*src*/, std::string_view event,
                                 const esl::ObservedEventData& data)
{
   if (event == sge::WindowResizedEvent)
   {
      const sge::WindowResizedData& resizeData =
         static_cast<const sge::WindowResizedData&>(data);
      onMainWindowResize(resizeData.newSize);
   }
}


void Towers::onMainWindowResize(const glm::ivec2& newSize)
{
   m_renderer.setFrustumSize(newSize.x, newSize.y);
}


void Towers::onInputChanged(sge::Input& /*src*/, std::string_view event,
                            const esl::ObservedEventData& data)
{
   if (event == sge::MouseMovedEvent)
   {
      const auto& movedData = static_cast<const sge::MouseMovedData&>(data);
      onMouseMoved(movedData.delta);
   }
   else if (event == sge::MouseScrolledEvent)
   {
      const auto& scrolledData = static_cast<const sge::MouseScrolledData&>(data);
      onMouseScrolled(scrolledData.delta);
   }
   else if (event == sge::MouseButtonChangedEvent)
   {
      const auto& buttonData = static_cast<const sge::MouseButtonChangedData&>(data);
      onMouseButtonChanged(buttonData.button, buttonData.action, buttonData.pos);
   }
   else if (event == sge::KeyPolledEvent)
   {
      const auto& polledData = static_cast<const sge::KeyPolledData&>(data);
      onKeyPolled(polledData.key, polledData.frameLengthSecs);
   }
}


void Towers::onMouseMoved(const glm::vec2& /*delta*/)
{
}


void Towers::onMouseScrolled(const glm::vec2& /*delta*/)
{
}


void Towers::onMouseButtonChanged(gfl::MouseButton button, int action,
                                  const glm::vec2& pos)
{
   if (button == GLFW_MOUSE_BUTTON_1)
   {
      if (action == GLFW_PRESS)
         onLeftButtonPressed(pos);
      else
         onLeftButtonReleased(pos);
   }
   else
   {
      if (action == GLFW_PRESS)
         onRightButtonPressed(pos);
      else
         onRightButtonReleased(pos);
   }
}


void Towers::onLeftButtonPressed(const glm::vec2& pos)
{
   if (mapOnLeftButtonPressed(pos))
      return;
   dashboardOnLeftButtonPressed(pos);
}


void Towers::onLeftButtonReleased(const glm::vec2& pos)
{
   if (mapOnLeftButtonReleased(pos))
      return;
   dashboardOnLeftButtonReleased(pos);
}


void Towers::onRightButtonPressed(const glm::vec2& /*pos*/)
{
}


void Towers::onRightButtonReleased(const glm::vec2& /*pos*/)
{
}


void Towers::onKeyPolled(gfl::Key /*key*/, float /*frameLengthSecs*/)
{
}


bool Towers::isInMap(const sge::PixPos& pos) const
{
   return pos.x > 0 && pos.x <= MapWidth && pos.y > 0 && pos.y <= MapHeight;
}


bool Towers::isInDashboard(const sge::PixPos& pos) const
{
   return pos.x > MapWidth && pos.x <= MapWidth + DashboardWidth && pos.y > 0 &&
          pos.y <= DashboardHeight;
}


bool Towers::mapOnLeftButtonPressed(const sge::PixPos& pos)
{
   if (!isInMap(pos))
      return false;

   if (m_placeSess)
   {
      placeDefender(pos);
      endPlaceSession();
   }

   return true;
}


bool Towers::mapOnLeftButtonReleased(const sge::PixPos& pos)
{
   if (!isInMap(pos))
      return false;
   return false;
}


bool Towers::dashboardOnLeftButtonPressed(const sge::PixPos& pos)
{
   if (!isInDashboard(pos))
      return false;

   const sge::PixPos posInDashboard = pos - sge::PixPos{MapWidth, 0.f};
   return m_dashboard.onLeftButtonPressed(posInDashboard);
}


bool Towers::dashboardOnLeftButtonReleased(const sge::PixPos& pos)
{
   if (!isInDashboard(pos))
      return false;

   const sge::PixPos posInDashboard = pos - sge::PixPos{MapWidth, 0.f};
   return m_dashboard.onLeftButtonReleased(posInDashboard);
}


bool Towers::canAffordDefender(const std::string& model) const
{
   const DefenderAttribs attribs = m_defenseFactory->defaultAttributes(model);
   return attribs.cost() <= m_credits;
}


bool Towers::isPaused() const
{
   return m_isPaused;
}


void Towers::startPlaceSession(std::string_view model, std::string_view indicatorTex,
                               sge::PixDim indicatorDim)
{
   PlaceSession sess;
   sess.model = model;
   sess.indicator =
      sge::Sprite{sge::SpriteLook{indicatorTex}, sge::SpriteForm{indicatorDim}};
   sess.range = m_defenseFactory->defaultAttributes(sess.model).range();

   m_placeSess = std::move(sess);
}


void Towers::endPlaceSession()
{
   m_placeSess = std::nullopt;
}


void Towers::startAttack()
{
   m_isPaused = false;
}


void Towers::pauseAttack()
{
   m_isPaused = true;
}