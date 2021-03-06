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
// Runtime
#include <iostream>
#include <numeric>
#include <thread>


namespace
{

///////////////////

constexpr float MouseSensitivity = 0.05f;
constexpr float ScrollSensitivity = 2.0f;
constexpr float MovementSpeed = 2.5f;


///////////////////

static sp::MapPos truncate(sp::MapPos pos)
{
   return sp::MapPos(static_cast<int>(pos.x), static_cast<int>(pos.y));
}


static sp::MapPos centerInField(sp::MapPos pos)
{
   return truncate(pos) + sp::MapVec{.5f, .5f};
}

} // namespace


///////////////////

Towers::Towers()
: Game2{WndWidth, WndHeight, "towers"}, m_dashboard{DashboardWidth, DashboardHeight, this,
                                                    this}
{
}


bool Towers::setup()
{
   const FileSysConfig fsConfig{m_paths.shaderPath(), m_paths.fontPath()};
   if (!Game2::setup(fsConfig))
      return false;
   if (!setupTextures())
      return false;
   if (!setupLevels())
      return false;
   return loadLevel(0);
}


void Towers::cleanup()
{
   Game2::cleanup();
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
      {Explosion1Texture, scene, "explosion1.png"},
      {Explosion2Texture, scene, "explosion2.png"},
      {Explosion3Texture, scene, "explosion3.png"},
      {Explosion4Texture, scene, "explosion4.png"},
      {Map1Texture, scene, "map1.png"},
      {Map2Texture, scene, "map2.png"},
      {InvalidFieldTexture, scene, "invalid_field.png"},
      {RangeTexture, scene, "range.png"},
      {HpStatusTexture, scene, "hp_status.png"},
      {DashboardTexture, ui, "dashboard.png"},
      {ButtonBackgroundTexture, ui, "defender_button_bgrd.png"},
      {StartTexture, ui, "start.png"},
      {PauseTexture, ui, "pause.png"},
      {VictoryTexture, ui, "victory.png"},
      {DefeatTexture, ui, "defeat.png"},
   };

   for (const auto& spec : textures)
      if (!resources().loadTexture(spec.tag, spec.path / spec.filename))
      {
         assert(false && "Unable to load texture.");
         return false;
      }

   return true;
}

bool Towers::setupGraphics()
{
   if (!setupRenderer())
      return false;
   if (!setupAnimations())
      return false;
   if (!setupAttackers())
      return false;
   if (!setupDefenders())
      return false;
   if (!setupSprites())
      return false;
   if (!m_dashboard.setup(renderer(), m_coordSys.get()))
      return false;
   return true;
}


bool Towers::setupRenderer()
{
   constexpr sp::NormDim hpStatusDim{.5f, .05f};
   const sp::PixDim hpStatusPixDim = toPix(hpStatusDim);
   constexpr sp::NormVec hpOffset{-.25f, -.4f};
   const sp::PixVec hpPixOffset = toPix(hpOffset);
   m_hpRenderer = std::make_unique<HpRenderer>(
      sp::Sprite{sp::SpriteLook{HpStatusTexture}, sp::SpriteForm{hpStatusPixDim}},
      hpPixOffset);

   return true;
}


bool Towers::setupAnimations()
{
   assert(!!m_coordSys);

   AnimationFactory factory;

   resources().addAnimation(
      ExplosionAnimation, factory.make(ExplosionAnimation, toPix(sp::MapDim{1.f, 1.f})));

   sp::PixDim firingSize =
      toPix(scaleInto(toMap(resources().getTextureSize(LtTexture)), LtSize));
   resources().addAnimation(LtFiringAnimation,
                            factory.make(LtFiringAnimation, firingSize));

   firingSize = toPix(scaleInto(toMap(resources().getTextureSize(SmTexture)), SmSize));
   resources().addAnimation(SmFiringAnimation,
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
      AttackerLook{sp::Sprite{sp::SpriteLook{AatTexture},
                              sp::SpriteForm{resources().getTextureSize(AatTexture)}},
                   sp::Sprite{sp::SpriteLook{AatHitTexture},
                              sp::SpriteForm{resources().getTextureSize(AatHitTexture)}},
                   resources().getAnimation(ExplosionAnimation), m_hpRenderer.get()},
      AssaultTank::defaultAttributes());
   m_attackFactory->registerModel(
      MhcModel,
      AttackerLook{sp::Sprite{sp::SpriteLook{MhcTexture},
                              sp::SpriteForm{resources().getTextureSize(MhcTexture)}},
                   sp::Sprite{sp::SpriteLook{MhcHitTexture},
                              sp::SpriteForm{resources().getTextureSize(MhcHitTexture)}},
                   resources().getAnimation(ExplosionAnimation), m_hpRenderer.get()},
      MobileCannon::defaultAttributes());

   return true;
}


bool Towers::setupDefenders()
{
   assert(!!m_coordSys);
   assert(!!m_map);

   m_defenseFactory = std::make_unique<DefenderFactory>(m_coordSys.get(), &m_attackers);

   m_defenseFactory->registerModel(
      LtModel,
      DefenderLook{sp::Sprite{sp::SpriteLook{LtTexture},
                              sp::SpriteForm{resources().getTextureSize(LtTexture)}},
                   resources().getAnimation(LtFiringAnimation)},
      LaserTurret::defaultAttributes());

   m_defenseFactory->registerModel(
      SmModel,
      DefenderLook{sp::Sprite{sp::SpriteLook{SmTexture},
                              sp::SpriteForm{resources().getTextureSize(SmTexture)}},
                   resources().getAnimation(SmFiringAnimation)},
      SonicMortar::defaultAttributes());

   resetDefenderPlacements();
   return true;
}


bool Towers::setupSprites()
{
   const sp::PixDim fieldPixDim = toPix(sp::MapDim{1.f, 1.f});
   m_invalidFieldOverlay =
      sp::Sprite{sp::SpriteLook{InvalidFieldTexture}, sp::SpriteForm{fieldPixDim}};
   m_rangeOverlay = sp::Sprite{sp::SpriteLook{RangeTexture}, sp::SpriteForm{fieldPixDim}};

   m_victoryDisplay =
      sp::Sprite{sp::SpriteLook{VictoryTexture},
                 sp::SpriteForm{resources().getTextureSize(VictoryTexture)}};
   m_defeatDisplay =
      sp::Sprite{sp::SpriteLook{DefeatTexture},
                 sp::SpriteForm{resources().getTextureSize(DefeatTexture)}};

   return true;
}


bool Towers::setupLevels()
{
   m_levels.push_back({170,
                       "map1.json",
                       Map1Texture,
                       {AttackerSpec{AatModel, sp::MapVec{0.f, 0.f}, DefaultDelay},
                        AttackerSpec{AatModel, sp::MapVec{0.f, 0.f}, 30},
                        AttackerSpec{MhcModel, sp::MapVec{-.08f, .05f}, DefaultDelay},
                        AttackerSpec{MhcModel, sp::MapVec{0.f, -0.05}, DefaultDelay}}});
   m_levels.push_back({100,
                       "map2.json",
                       Map2Texture,
                       {AttackerSpec{MhcModel, sp::MapVec{-.08f, .05f}, DefaultDelay},
                        AttackerSpec{MhcModel, sp::MapVec{0.f, -0.05}, 20},
                        AttackerSpec{MhcModel, sp::MapVec{0.06f, -0.05}, 40},
                        AttackerSpec{MhcModel, sp::MapVec{0.1f, 0.02}, 60},
                        AttackerSpec{MhcModel, sp::MapVec{-0.1f, 0.1}, 80},
                        AttackerSpec{AatModel, sp::MapVec{0.f, 0.f}, 200},
                        AttackerSpec{AatModel, sp::MapVec{0.05f, 0.03f}, 205},
                        AttackerSpec{AatModel, sp::MapVec{-0.07f, -0.1f}, 210},
                        AttackerSpec{AatModel, sp::MapVec{0.03f, -0.05f}, 215},
                        AttackerSpec{AatModel, sp::MapVec{0.07f, 0.06f}, 220},
                        AttackerSpec{AatModel, sp::MapVec{-0.03f, 0.f}, 220},
                        AttackerSpec{AatModel, sp::MapVec{0.f, -0.03f}, 220}}});
   return true;
}


bool Towers::loadLevel(std::size_t level)
{
   const Level& l = m_levels[level];

   m_hasLost = false;
   m_hasWonLevel = false;
   m_credits = l.credits;

   if (!loadMap(l.mapFileName))
      return false;

   m_background =
      sp::Sprite{sp::SpriteLook{l.backgroundTex}, sp::SpriteForm{{MapWidth, MapHeight}}};

   // Recalc all graphics that depend on the map size.
   if (!setupGraphics())
      return false;

   for (const auto& spec : l.attackers)
      addAttacker(m_attackFactory->makeAttacker(
         spec.model, OffsetPath{&m_map->path(), spec.pathOffset}, spec.launchDelay));
   m_defenders.clear();

   setPaused(true);
   return true;
}


bool Towers::loadMap(const std::string& fileName)
{
   std::optional<MapData> mapData = loadMapData(m_paths.mapPath() / fileName);
   if (!mapData)
   {
      assert(false && "Unable to load map.");
      return false;
   }
   m_map = std::make_unique<Map>(std::move(*mapData));

   const sp::IntDim mapSizeInFields = m_map->sizeInFields();
   const sp::PixDim fieldPixDim{MapWidth / mapSizeInFields.x,
                                MapHeight / mapSizeInFields.y};
   m_coordSys = std::make_unique<MapCoordSys>(fieldPixDim);

   return true;
}


void Towers::updateState()
{
   if (isPaused() || m_hasLost)
      return;

   for (auto& entry : m_attackers)
      entry.second.update();
   for (auto& defender : m_defenders)
      defender.update();

   const bool haveFinishedAttacker =
      std::any_of(begin(m_attackers), end(m_attackers), [](const auto& attackerEntry) {
         return attackerEntry.second.hasFinished();
      });
   removeDestroyedOrFinishedAttackers();

   m_hasLost |= haveFinishedAttacker;
   m_hasWonLevel = !m_hasLost && m_attackers.empty();
   if (m_hasWonLevel)
   {
      if (m_currLevel < m_levels.size() - 1)
         advanceLevel();
      else
         m_hasWonGame = true;
   }
}


void Towers::renderItems()
{
   renderMap();
   m_dashboard.render(renderer(), sp::PixPos{MapWidth - 1, 0.f});

   for (auto& entry : m_attackers)
      entry.second.render(renderer(), isPaused());
   for (auto& defender : m_defenders)
      defender.render(renderer(), isPaused());

   if (m_hasWonGame)
   {
      renderVictoryDisplay();
   }
   else if (m_hasLost)
   {
      renderDefeatDisplay();
   }
   else
   {
      renderDefenderInfo();
      // Draw placed content on top of everything else.
      renderPlaceSession();
   }
}


void Towers::renderMap()
{
   renderer().renderSprite(m_background, {0.f, 0.f});
}


void Towers::renderDefenderInfo()
{
   if (m_placeSess)
      return;

   const sp::PixPos mousePixPos{mousePosition()};
   const sp::MapPos fieldPos = truncate(toMap(mousePixPos));
   if (!m_map->isOnMap(fieldPos))
      return;

   const Defender* touchedDefender = defenderOnField(fieldPos);
   if (touchedDefender)
   {
      const sp::MapCoord range = touchedDefender->range();
      const sp::PixDim rangeDim = toPix(2.0f * sp::MapDim{range, range});
      const sp::MapPos at = touchedDefender->center();
      const sp::PixPos atPix = toPix(at);
      m_rangeOverlay.setSize(rangeDim);
      renderer().renderSpriteCentered(m_rangeOverlay, atPix);
   }
}


void Towers::renderPlaceSession()
{
   if (m_placeSess)
   {
      const sp::PixPos mousePixPos{mousePosition()};

      const sp::MapPos fieldPos = truncate(toMap(mousePixPos));
      const sp::PixPos fieldPixPos = toPix(fieldPos);
      const sp::PixPos fieldCenterPixPos = fieldPixPos + toPix(sp::MapPos{.5f, .5f});

      const bool isOnMap = m_map->isOnMap(fieldPos);
      if (isOnMap)
      {
         if (canPlaceDefenderOnField(m_placeSess->model, fieldPos))
         {
            const sp::PixDim rangeDim =
               toPix(2.0f * sp::MapDim{m_placeSess->range, m_placeSess->range});
            m_rangeOverlay.setSize(rangeDim);
            renderer().renderSpriteCentered(m_rangeOverlay, fieldCenterPixPos);
         }
         else
         {
            renderer().renderSprite(m_invalidFieldOverlay, fieldPixPos);
         }
      }

      const sp::PixPos indicatorCenter = isOnMap ? fieldCenterPixPos : mousePixPos;
      renderer().renderSpriteCentered(m_placeSess->indicator, indicatorCenter);
   }
}


void Towers::renderVictoryDisplay()
{
   const sp::PixPos pos =
      (sp::PixPos{MapWidth, MapHeight} - m_victoryDisplay.size()) / 2.f;
   renderer().renderSprite(m_victoryDisplay, pos);
}


void Towers::renderDefeatDisplay()
{
   const sp::PixPos pos =
      (sp::PixPos{MapWidth, MapHeight} - m_defeatDisplay.size()) / 2.f;
   renderer().renderSprite(m_defeatDisplay, pos);
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


void Towers::removeDestroyedOrFinishedAttackers()
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
   const sp::IntDim mapDim = m_map->sizeInFields();
   m_defenderMatrix.resize(mapDim.x * mapDim.y);
   std::fill(m_defenderMatrix.begin(), m_defenderMatrix.end(), false);
}


bool Towers::hasDefenderOnField(sp::MapPos field) const
{
   const int idx =
      static_cast<int>(field.y) * m_map->sizeInFields().x + static_cast<int>(field.x);
   assert(idx >= 0 && idx < m_defenderMatrix.size());
   return m_defenderMatrix[idx];
}


const Defender* Towers::defenderOnField(sp::MapPos field) const
{
   if (hasDefenderOnField(field))
      for (const auto& defender : m_defenders)
         if (m_map->isOnSameField(defender.center(), field))
            return &defender;
   return nullptr;
}


void Towers::setDefenderOnField(sp::MapPos field, bool hasDefender)
{
   const int idx =
      static_cast<int>(field.y) * m_map->sizeInFields().x + static_cast<int>(field.x);
   assert(idx >= 0 && idx < m_defenderMatrix.size());
   m_defenderMatrix[idx] = hasDefender;
}


bool Towers::canPlaceDefenderOnField(const std::string& /*defenderModel*/,
                                     sp::MapPos field) const
{
   return m_map->canBuildOnField(field) && !hasDefenderOnField(field);
}


void Towers::addDefender(std::optional<Defender>&& defender, const sp::PixPos& pos)
{
   if (defender)
   {
      m_credits -= defender->cost();
      m_defenders.push_back(*defender);
      setDefenderOnField(pos, true);
   }
}


void Towers::placeDefender(const sp::PixPos& mousePos)
{
   const sp::MapPos pos = centerInField(toMap(mousePos));
   if (!m_map->isOnMap(pos) || !canPlaceDefenderOnField(m_placeSess->model, pos))
      return;

   addDefender(m_defenseFactory->makeDefender(m_placeSess->model, pos), pos);
}


void Towers::playLevel(std::size_t level)
{
   m_hasLost = false;
   m_hasWonLevel = false;
   m_hasWonGame = false;

   m_currLevel = level;
   loadLevel(level);
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


void Towers::onKeyPressed(gfl::Key key, int mods)
{
   if (victoryDisplayOnKeyPressed(key, mods))
      return;
   if (defeatDisplayOnKeyPressed(key, mods))
      return;
}


bool Towers::isInMap(const sp::PixPos& pos) const
{
   return pos.x > 0 && pos.x <= MapWidth && pos.y > 0 && pos.y <= MapHeight;
}


bool Towers::isInDashboard(const sp::PixPos& pos) const
{
   return pos.x > MapWidth && pos.x <= MapWidth + DashboardWidth && pos.y > 0 &&
          pos.y <= DashboardHeight;
}


bool Towers::mapOnLeftButtonPressed(const sp::PixPos& pos)
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


bool Towers::mapOnLeftButtonReleased(const sp::PixPos& pos)
{
   if (!isInMap(pos))
      return false;
   return false;
}


bool Towers::dashboardOnLeftButtonPressed(const sp::PixPos& pos)
{
   if (!isInDashboard(pos))
      return false;

   const sp::PixPos posInDashboard = pos - sp::PixPos{MapWidth, 0.f};
   return m_dashboard.onLeftButtonPressed(posInDashboard);
}


bool Towers::dashboardOnLeftButtonReleased(const sp::PixPos& pos)
{
   if (!isInDashboard(pos))
      return false;

   const sp::PixPos posInDashboard = pos - sp::PixPos{MapWidth, 0.f};
   return m_dashboard.onLeftButtonReleased(posInDashboard);
}


bool Towers::victoryDisplayOnKeyPressed(gfl::Key key, int /*mods*/)
{
   if (m_hasWonGame)
   {
      switch (key)
      {
      case GLFW_KEY_Y:
         playAgain();
         return true;
      case GLFW_KEY_N:
         quit();
         return true;
      }
   }

   return false;
}


bool Towers::defeatDisplayOnKeyPressed(gfl::Key key, int /*mods*/)
{
   if (m_hasLost)
   {
      switch (key)
      {
      case GLFW_KEY_Y:
         repeatLevel();
         return true;
      case GLFW_KEY_N:
         quit();
         return true;
      }
   }

   return false;
}


bool Towers::canAffordDefender(const std::string& model) const
{
   const DefenderAttribs attribs = m_defenseFactory->defaultAttributes(model);
   return attribs.cost() <= m_credits;
}


bool Towers::isPaused() const
{
   return Game2::isPaused();
}


void Towers::startPlaceSession(std::string_view model, std::string_view indicatorTex,
                               sp::PixDim indicatorDim)
{
   PlaceSession sess;
   sess.model = model;
   sess.indicator =
      sp::Sprite{sp::SpriteLook{indicatorTex}, sp::SpriteForm{indicatorDim}};
   sess.range = m_defenseFactory->defaultAttributes(sess.model).range();

   m_placeSess = std::move(sess);
}


void Towers::endPlaceSession()
{
   m_placeSess = std::nullopt;
}


void Towers::startAttack()
{
   setPaused(false);
}


void Towers::pauseAttack()
{
   setPaused(true);
}


void Towers::playAgain()
{
   playLevel(0);
}


void Towers::advanceLevel()
{
   playLevel(m_currLevel + 1);
}


void Towers::repeatLevel()
{
   playLevel(m_currLevel);
}


void Towers::quit()
{
   mainWnd().setShouldClose(true);
}
