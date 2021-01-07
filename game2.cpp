//
// Nov-2020, Michael Lindner
// MIT license
//
#include "game2.h"
#include "animation_factory.h"
#include "animation_tags.h"
#include "attacker_models.h"
#include "defender_models.h"
#include "map_data.h"
#include "mesh2.h"
#include "texture_tags.h"
#include "gll_debug.h"
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

static MapPos truncate(MapPos pos)
{
   return MapPos(static_cast<int>(pos.x), static_cast<int>(pos.y));
}


static MapPos centerInField(MapPos pos)
{
   return truncate(pos) + MapVec{.5f, .5f};
}

} // namespace


///////////////////

Game2::Game2()
: m_glfw{OpenGLVersion, GLFW_OPENGL_CORE_PROFILE}, m_dashboard{DashboardWidth,
                                                               DashboardHeight, this}
{
}


bool Game2::setup()
{
   return (setupUi() && setupInput() && setupOutput() && setupTextures() &&
           setupRenderer() && setupTerrain() && setupSpriteData() && setupAnimations() &&
           setupAttackers() && setupDefenders() && setupBackground() &&
           m_dashboard.setup(m_spriteRenderer.get(), m_coordSys.get()));
}


void Game2::cleanup()
{
}


void Game2::run()
{
   m_frameClock.nextLap();
   float processLag = 0.f;

   while (!m_mainWnd.shouldClose())
   {
      m_frameClock.nextLap();
      processLag += m_frameClock.lapLength();

      processInput();

      // Catch up with the game state for the time that was used for rendering and
      // processing input. This standardizes the game speed across machines with
      // various processing speeds.
      constexpr float UpdateGranularityMs = 25.f;
      while (processLag >= UpdateGranularityMs)
      {
         updateState();
         processLag -= UpdateGranularityMs;
      }

      render();
   }
}


bool Game2::setupUi()
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


bool Game2::setupMainWindow()
{
   m_mainWnd.setInputController(&m_input);
   m_mainWnd.addObserver([this](MainWindow& src, std::string_view msg,
                                const Observed<MainWindow>::MsgData& data) {
      onMainWindowChanged(src, msg, data);
   });

   if (m_mainWnd.create(WndWidth, WndHeight, "towers") != GLFW_NO_ERROR)
      return false;

   // Hide mouse cursor and capture its events even outside the main window.
   // m_mainWnd.setCursorMode(GLFW_CURSOR_DISABLED);
   return true;
}


bool Game2::setupInput()
{
   m_input.addObserver(
      [this](Input& /*src*/, std::string_view msg, const Observed<Input>::MsgData& data) {
         onInputChanged(m_input, msg, data);
      });
   return true;
}


bool Game2::setupOutput()
{
   if (gll::haveDebugContext())
      gll::setDebugOutputCallback();
   return true;
}


bool Game2::setupTextures()
{
   struct TextureSpec
   {
      std::string tag;
      std::filesystem::path filename;
   };
   const std::vector<TextureSpec> textures{
      {AatTexture, "aat.png"},
      {MhcTexture, "mhc.png"},
      {LtTexture, "lt.png"},
      {LtFiring1Texture, "lt_firing1.png"},
      {LtFiring2Texture, "lt_firing2.png"},
      {SmTexture, "sm.png"},
      {SmFiring1Texture, "sm_firing1.png"},
      {SmFiring2Texture, "sm_firing2.png"},
      {Explosion1TTag, "explosion1.png"},
      {Explosion2TTag, "explosion2.png"},
      {Explosion3TTag, "explosion3.png"},
      {Explosion4TTag, "explosion4.png"},
      {Map1TTag, "map1.png"},
      {ValidFieldTTag, "valid_field.png"},
      {InvalidFieldTTag, "invalid_field.png"},
      {DashboardTTag, "dashboard.png"},
   };

   for (const auto& spec : textures)
      if (!m_resources.loadTexture(spec.tag, m_resources.texturePath() / spec.filename))
         return false;
   return true;
}


bool Game2::setupRenderer()
{
   return m_renderer.setup(&m_resources, WndWidth, WndHeight);
}


bool Game2::setupTerrain()
{
   std::optional<MapData> mapData = loadMapData(m_resources.mapPath() / "map.json");
   if (!mapData)
      return false;
   m_map = std::make_unique<Map>(std::move(*mapData));

   const PixDim mapPixDim{MapWidth, MapHeight};
   const IntDim mapSizeInFields = m_map->sizeInFields();
   const PixDim fieldPixDim{mapPixDim.x / mapSizeInFields.x,
                            mapPixDim.y / mapSizeInFields.y};
   m_coordSys = std::make_unique<MapCoordSys>(fieldPixDim);

   return true;
}


bool Game2::setupSpriteData()
{
   // Coord system for vertex coordinates is:
   // (0, 0) - left-top, (1, 1) - right-bottom
   const std::vector<Mesh2::Point> positions = {
      {0.f, 1.f}, {1.f, 1.f}, {1.f, 0.f}, {0.f, 0.f}};
   // Triangle vertices are ordered ccw.
   const std::vector<Mesh2::VertexIdx> indices = {0, 1, 2, 2, 3, 0};
   // Coord system for texture coordinates is:
   // (0, 0) - left-bottom, (1, 1) - right-top
   const std::vector<Mesh2::Point> texCoords = positions;

   Mesh2 mesh;
   mesh.setPositions(positions);
   mesh.setIndices(indices);
   mesh.setTextureCoords(texCoords);
   m_spriteRenderer = std::make_unique<SpriteRenderer>(&m_resources);
   m_spriteRenderer->setMesh(mesh);

   return true;
}


bool Game2::setupAnimations()
{
   assert(!!m_coordSys);
   assert(!!m_spriteRenderer);

   AnimationFactory factory(m_spriteRenderer.get());

   m_resources.addAnimation(
      ExplosionATag,
      factory.make(ExplosionATag, m_coordSys->toRenderCoords(MapDim{1.f, 1.f})));

   PixDim firingSize = m_coordSys->toRenderCoords(
      m_coordSys->scaleToSize(LtSize, m_resources.getTextureSize(LtTexture)));
   m_resources.addAnimation(LtFiringAnimation,
                            factory.make(LtFiringAnimation, firingSize));

   firingSize = m_coordSys->toRenderCoords(
      m_coordSys->scaleToSize(SmSize, m_resources.getTextureSize(SmTexture)));
   m_resources.addAnimation(SmFiringAnimation,
                            factory.make(SmFiringAnimation, firingSize));

   return true;
}


static void addAttacker(const std::optional<Attacker>& attacker,
                        std::vector<Attacker>& coll)
{
   if (attacker)
      coll.push_back(*attacker);
}


bool Game2::setupAttackers()
{
   assert(!!m_coordSys);
   assert(!!m_map);
   assert(!!m_spriteRenderer);

   m_attackFactory = std::make_unique<AttackerFactory>(m_coordSys.get());

   m_attackFactory->registerModel(
      AatModel, AttackerLook{Sprite{m_spriteRenderer.get(), SpriteLook{AatTexture},
                                    SpriteForm{m_resources.getTextureSize(AatTexture)}},
                             m_resources.getAnimation(ExplosionATag)});
   m_attackFactory->registerModel(
      MhcModel, AttackerLook{Sprite{m_spriteRenderer.get(), SpriteLook{MhcTexture},
                                    SpriteForm{m_resources.getTextureSize(MhcTexture)}},
                             m_resources.getAnimation(ExplosionATag)});

   addAttacker(
      m_attackFactory->makeAttacker(AatModel, .8f, Attacker::Attribs{2000, .03f, 0},
                                    OffsetPath{&m_map->path(), MapVec{0.f, 0.f}}),
      m_attackers);
   addAttacker(
      m_attackFactory->makeAttacker(AatModel, .8f, Attacker::Attribs{2000, .03f, 100},
                                    OffsetPath{&m_map->path(), MapVec{0.f, 0.f}}),
      m_attackers);
   addAttacker(
      m_attackFactory->makeAttacker(MhcModel, .4f, Attacker::Attribs{800, .05f, 0},
                                    OffsetPath{&m_map->path(), MapVec{-.08f, .05f}}),
      m_attackers);
   addAttacker(
      m_attackFactory->makeAttacker(MhcModel, .4f, Attacker::Attribs{800, .05f, 10},
                                    OffsetPath{&m_map->path(), MapVec{0.f, -0.05}}),
      m_attackers);

   return true;
}


static void addDefender(const std::optional<Defender>& defender,
                        std::vector<Defender>& coll)
{
   if (defender)
      coll.push_back(*defender);
}


bool Game2::setupDefenders()
{
   assert(!!m_coordSys);
   assert(!!m_map);
   assert(!!m_spriteRenderer);

   m_defenseFactory = std::make_unique<DefenderFactory>(m_coordSys.get(), &m_attackers);

   m_defenseFactory->registerModel(
      LtModel, DefenderLook{Sprite{m_spriteRenderer.get(), SpriteLook{LtTexture},
                                   SpriteForm{m_resources.getTextureSize(LtTexture)}},
                            m_resources.getAnimation(LtFiringAnimation)});

   m_defenseFactory->registerModel(
      SmModel, DefenderLook{Sprite{m_spriteRenderer.get(), SpriteLook{SmTexture},
                                   SpriteForm{m_resources.getTextureSize(SmTexture)}},
                            m_resources.getAnimation(SmFiringAnimation)});

   resetDefenderPlacements();
   return true;
}


bool Game2::setupBackground()
{
   assert(!!m_spriteRenderer);
   m_background = std::make_unique<Sprite>(m_spriteRenderer.get(), SpriteLook{Map1TTag},
                                           SpriteForm{{MapWidth, MapHeight}});

   const PixDim fieldPixDim = m_coordSys->toRenderCoords(MapDim{1.f, 1.f});
   m_validFieldOverlay = std::make_unique<Sprite>(
      m_spriteRenderer.get(), SpriteLook{ValidFieldTTag}, SpriteForm{fieldPixDim});
   m_invalidFieldOverlay = std::make_unique<Sprite>(
      m_spriteRenderer.get(), SpriteLook{InvalidFieldTTag}, SpriteForm{fieldPixDim});

   return true;
}


void Game2::processInput()
{
   m_input.process(m_mainWnd, m_frameClock.lapLength(MsToSecs));
}


void Game2::updateState()
{
   for (auto& attacker : m_attackers)
      attacker.update();
   for (auto& defender : m_defenders)
      defender.update();

   // Remove destroyed attackers.
   m_attackers.erase(
      std::remove_if(m_attackers.begin(), m_attackers.end(),
                     [](const auto& attacker) { return attacker.canBeRemoved(); }),
      m_attackers.end());
}


void Game2::render()
{
   m_renderer.beginRendering(true);
   renderMap();
   m_dashboard.render(m_renderer.shaders(), PixPos{MapWidth - 1, 0.f});

   for (auto& attacker : m_attackers)
      attacker.render(m_renderer.shaders());
   for (auto& defender : m_defenders)
      defender.render(m_renderer.shaders());

   // Draw placed content on top of everything else.
   renderPlaceSession();

   m_mainWnd.swapBuffers();
}


void Game2::renderMap()
{
   m_background->render(m_renderer.shaders(), {0.f, 0.f});
}


void Game2::renderPlaceSession()
{
   if (m_placeSess)
   {
      double x = 0.;
      double y = 0.;
      glfwGetCursorPos(m_mainWnd.handle(), &x, &y);
      const PixPos mousePixPos(x, y);

      const MapPos fieldPos = truncate(m_coordSys->toMapCoords(mousePixPos));
      const PixPos fieldPixPos = m_coordSys->toRenderCoords(fieldPos);
      const PixPos fieldCenterPixPos =
         fieldPixPos + m_coordSys->toRenderCoords(MapPos{.5f, .5f});

      const bool isOnMap = m_map->isOnMap(fieldPos);
      if (isOnMap)
      {
         const Sprite& overlay = canPlaceDefenderOnField(m_placeSess->model, fieldPos)
                                    ? *m_validFieldOverlay
                                    : *m_invalidFieldOverlay;
         overlay.render(m_renderer.shaders(), fieldPixPos);
      }

      const PixDim halfIndicatorSize = m_placeSess->indicator->size() / 2.f;
      const PixPos indicatorCenter = isOnMap ? fieldCenterPixPos : mousePixPos;
      m_placeSess->indicator->render(m_renderer.shaders(),
                                     indicatorCenter - halfIndicatorSize);
   }
}


void Game2::resetDefenderPlacements()
{
   assert(m_map);
   const IntDim mapDim = m_map->sizeInFields();
   m_defenderMatrix.resize(mapDim.x * mapDim.y);
   std::fill(m_defenderMatrix.begin(), m_defenderMatrix.end(), false);
}


bool Game2::hasDefenderOnField(MapPos field) const
{
   const int idx =
      static_cast<int>(field.y) * m_map->sizeInFields().x + static_cast<int>(field.x);
   assert(idx >= 0 && idx < m_defenderMatrix.size());
   return m_defenderMatrix[idx];
}


void Game2::setDefenderOnField(MapPos field, bool hasDefender)
{
   const int idx =
      static_cast<int>(field.y) * m_map->sizeInFields().x + static_cast<int>(field.x);
   assert(idx >= 0 && idx < m_defenderMatrix.size());
   m_defenderMatrix[idx] = hasDefender;
}


bool Game2::canPlaceDefenderOnField(const std::string& /*defenderModel*/,
                                    MapPos field) const
{
   return m_map->canBuildOnField(field) && !hasDefenderOnField(field);
}


void Game2::placeDefender(const PixPos& mousePos)
{
   const MapPos pos = centerInField(m_coordSys->toMapCoords(mousePos));
   if (!m_map->isOnMap(pos) || !canPlaceDefenderOnField(m_placeSess->model, pos))
   {
      return;
   }

   if (m_placeSess->model == LtModel)
   {
      constexpr LaserTurret::Attribs attribs{LtRange, LtDamage};
      addDefender(m_defenseFactory->makeLaserTurret(LtSize, pos, attribs), m_defenders);
      setDefenderOnField(pos, true);
   }
   else if (m_placeSess->model == SmModel)
   {
      constexpr SonicMortar::Attribs attribs{SmRange, SmDamage};
      addDefender(m_defenseFactory->makeSonicMortar(SmSize, pos, attribs), m_defenders);
      setDefenderOnField(pos, true);
   }
}


void Game2::onMainWindowChanged(MainWindow& /*src*/, std::string_view msg,
                                const Observed<MainWindow>::MsgData& data)
{
   if (msg == WindowResizedMsg)
   {
      const WindowResizedMsgData& resizeData =
         static_cast<const WindowResizedMsgData&>(data);
      onMainWindowResize(resizeData.newSize);
   }
}


void Game2::onMainWindowResize(const glm::ivec2& newSize)
{
   m_renderer.setFrustumSize(newSize.x, newSize.y);
}


void Game2::onInputChanged(Input& /*src*/, std::string_view msg,
                           const Observed<Input>::MsgData& data)
{
   if (msg == MouseMovedMsg)
   {
      const auto& movedData = static_cast<const MouseMovedMsgData&>(data);
      onMouseMoved(movedData.delta);
   }
   else if (msg == MouseScrolledMsg)
   {
      const auto& scrolledData = static_cast<const MouseScrolledMsgData&>(data);
      onMouseScrolled(scrolledData.delta);
   }
   else if (msg == MouseButtonChangedMsg)
   {
      const auto& buttonData = static_cast<const MouseButtonChangedMsgData&>(data);
      onMouseButtonChanged(buttonData.button, buttonData.action, buttonData.pos);
   }
   else if (msg == KeyPolledMsg)
   {
      const auto& polledData = static_cast<const KeyPolledMsgData&>(data);
      onKeyPolled(polledData.key, polledData.frameLengthSecs);
   }
}


void Game2::onMouseMoved(const glm::vec2& /*delta*/)
{
}


void Game2::onMouseScrolled(const glm::vec2& /*delta*/)
{
}


void Game2::onMouseButtonChanged(gfl::MouseButton button, int action,
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


void Game2::onLeftButtonPressed(const glm::vec2& pos)
{
   if (mapOnLeftButtonPressed(pos))
      return;
   dashboardOnLeftButtonPressed(pos);
}


void Game2::onLeftButtonReleased(const glm::vec2& pos)
{
   if (mapOnLeftButtonReleased(pos))
      return;
   dashboardOnLeftButtonReleased(pos);
}


void Game2::onRightButtonPressed(const glm::vec2& /*pos*/)
{
}


void Game2::onRightButtonReleased(const glm::vec2& /*pos*/)
{
}


void Game2::onKeyPolled(gfl::Key /*key*/, float /*frameLengthSecs*/)
{
}


bool Game2::isInMap(const PixPos& pos) const
{
   return pos.x > 0 && pos.x <= MapWidth && pos.y > 0 && pos.y <= MapHeight;
}


bool Game2::isInDashboard(const PixPos& pos) const
{
   return pos.x > MapWidth && pos.x <= MapWidth + DashboardWidth && pos.y > 0 &&
          pos.y <= DashboardHeight;
}


bool Game2::mapOnLeftButtonPressed(const PixPos& pos)
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


bool Game2::mapOnLeftButtonReleased(const PixPos& pos)
{
   if (!isInMap(pos))
      return false;
   return false;
}


bool Game2::dashboardOnLeftButtonPressed(const PixPos& pos)
{
   if (!isInDashboard(pos))
      return false;

   const PixPos posInDashboard = pos - PixPos{MapWidth, 0.f};
   return m_dashboard.onLeftButtonPressed(posInDashboard);
}


bool Game2::dashboardOnLeftButtonReleased(const PixPos& pos)
{
   if (!isInDashboard(pos))
      return false;

   const PixPos posInDashboard = pos - PixPos{MapWidth, 0.f};
   return m_dashboard.onLeftButtonReleased(posInDashboard);
}


void Game2::startPlaceSession(std::string_view model, std::string_view indicatorTex,
                              PixDim indicatorDim)
{
   PlaceSession sess;
   sess.model = model;
   sess.indicator = std::make_unique<Sprite>(
      m_spriteRenderer.get(), SpriteLook{indicatorTex}, SpriteForm{indicatorDim});

   m_placeSess = std::move(sess);
}


void Game2::endPlaceSession()
{
   m_placeSess = std::nullopt;
}
