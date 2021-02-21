//
// Feb-2021, Michael Lindner
// MIT license
//
#pragma once
#include "attacker.h"
#include "attacker_factory.h"
#include "commands.h"
#include "dashboard.h"
#include "defender.h"
#include "defender_factory.h"
#include "hp_renderer.h"
#include "level.h"
#include "map.h"
#include "map_coord_sys.h"
#include "paths.h"
#include "place_session.h"
#include "state.h"
#include "towers_types.h"
#include "ui_scale.h"
#include "spiel/coords.h"
#include "spiel/game2.h"
#include "spiel/sprite.h"
#include <memory>
#include <optional>
#include <unordered_map>
#include <vector>


class Towers : public sp::Game2, private Commands, private State
{
 public:
   Towers();

   bool setup();
   void cleanup();

 private:
   bool setupTextures();
   bool setupLevels();

   bool loadLevel(std::size_t level);
   bool loadMap(const std::string& fileName, sp::PixCoordi width, sp::PixCoordi height);
   bool setupGraphics();
   bool setupRenderer();
   bool setupAnimations();
   bool setupAttackers();
   bool setupDefenders();
   bool setupSprites();

   void updateState();
   void renderItems() override;
   void renderMap();
   void renderDefenderInfo();
   void renderPlaceSession();

   void addAttacker(std::optional<Attacker>&& attacker);
   template <typename SpecificAttacker>
   void onAttackerDestroyed(SpecificAttacker& src, std::string_view event,
                            const esl::ObservedEventData& data);
   void removeDestroyedOrFinishedAttackers();
   void removeAsTarget(EntityId attackerId);

   void resetDefenderPlacements();
   bool hasDefenderOnField(sp::MapPos field) const;
   const Defender* defenderOnField(sp::MapPos field) const;
   void setDefenderOnField(sp::MapPos field, bool hasDefender);
   bool canPlaceDefenderOnField(const std::string& defenderModel, sp::MapPos field) const;
   void addDefender(std::optional<Defender>&& defender, const sp::PixPos& pos);
   void placeDefender(const sp::PixPos& mousePos);

   void onLeftButtonPressed(const glm::vec2& pos) override;
   void onLeftButtonReleased(const glm::vec2& pos) override;

   bool isInMap(const sp::PixPos& pos) const;
   bool isInDashboard(const sp::PixPos& pos) const;
   bool mapOnLeftButtonPressed(const sp::PixPos& pos);
   bool mapOnLeftButtonReleased(const sp::PixPos& pos);
   bool dashboardOnLeftButtonPressed(const sp::PixPos& pos);
   bool dashboardOnLeftButtonReleased(const sp::PixPos& pos);

   // State overrides.
   int credits() const override { return m_credits; }
   bool canAffordDefender(const std::string& model) const override;
   bool isPaused() const override;

   // Commands overrides.
   void startPlaceSession(std::string_view model, std::string_view indicatorTex,
                          sp::PixDim indicatorDim) override;
   void endPlaceSession() override;
   void startAttack() override;
   void pauseAttack() override;

   // Convenience functions for coordinate system operations.
   sp::PixPos toPix(sp::MapPos mpos) const;
   sp::MapPos toMap(sp::PixPos ppos) const;
   sp::MapDim scaleInto(sp::MapDim source, sp::MapDim dest) const;

 private:
   static constexpr sp::PixCoordi MapWidth = UiScale(1800);
   static constexpr sp::PixCoordi MapHeight = UiScale(1200);
   static constexpr sp::PixCoordi DashboardWidth = UiScale(200);
   static constexpr sp::PixCoordi DashboardHeight = MapHeight;
   static constexpr sp::PixCoordi WndWidth = MapWidth + DashboardWidth;
   static constexpr sp::PixCoordi WndHeight = MapHeight;

   std::unique_ptr<MapCoordSys> m_coordSys;
   Paths m_paths;
   std::unique_ptr<HpRenderer> m_hpRenderer;
   std::unique_ptr<AttackerFactory> m_attackFactory;
   AttackerMap m_attackers;
   std::unique_ptr<DefenderFactory> m_defenseFactory;
   std::vector<Defender> m_defenders;
   std::vector<Level> m_levels;
   std::size_t m_currLevel = 0;
   std::unique_ptr<Map> m_map;
   sp::Sprite m_background;
   Dashboard m_dashboard;
   std::optional<PlaceSession> m_placeSess;
   sp::Sprite m_invalidFieldOverlay;
   sp::Sprite m_rangeOverlay;
   std::vector<bool> m_defenderMatrix;
   int m_credits = 0;
   bool m_hasLost = false;
   bool m_hasWonLevel = false;
};


inline sp::PixPos Towers::toPix(sp::MapPos mpos) const
{
   return m_coordSys->toPix(mpos);
}

inline sp::MapPos Towers::toMap(sp::PixPos ppos) const
{
   return m_coordSys->toMap(ppos);
}

inline sp::MapDim Towers::scaleInto(sp::MapDim source, sp::MapDim dest) const
{
   return m_coordSys->scaleInto(source, dest);
}
