//
// Feb-2021, Michael Lindner
// MIT license
//
#pragma once
#include "attacker.h"
#include "attacker_factory.h"
#include "basic_types.h"
#include "commands.h"
#include "dashboard.h"
#include "defender.h"
#include "defender_factory.h"
#include "hp_renderer.h"
#include "map.h"
#include "map_coord_sys.h"
#include "paths.h"
#include "place_session.h"
#include "state.h"
#include "sge_coords.h"
#include "sge_game2.h"
#include "sge_sprite.h"
#include "sge_types.h"
#include <memory>
#include <optional>
#include <unordered_map>
#include <vector>



class Towers : public sge::Game2, private Commands, private State
{
public:
   Towers();

   bool setup() override;
   void cleanup();

 private:
   bool setupTextures();
   bool setupTerrain();
   bool setupRenderer();
   bool setupAnimations();
   bool setupAttackers();
   bool setupDefenders();
   bool setupBackground();

   void updateState();
   void renderItems() override;
   void renderMap();
   void renderDefenderInfo();
   void renderPlaceSession();

   void addAttacker(std::optional<Attacker>&& attacker);
   template <typename SpecificAttacker>
   void onAttackerDestroyed(SpecificAttacker& src, std::string_view event,
                            const esl::ObservedEventData& data);
   void removeDestroyedAttackers();
   void removeAsTarget(EntityId attackerId);

   void resetDefenderPlacements();
   bool hasDefenderOnField(sge::MapPos field) const;
   const Defender* defenderOnField(sge::MapPos field) const;
   void setDefenderOnField(sge::MapPos field, bool hasDefender);
   bool canPlaceDefenderOnField(const std::string& defenderModel,
                                sge::MapPos field) const;
   void addDefender(std::optional<Defender>&& defender, const sge::PixPos& pos);
   void placeDefender(const sge::PixPos& mousePos);

   void onLeftButtonPressed(const glm::vec2& pos) override;
   void onLeftButtonReleased(const glm::vec2& pos) override;

   bool isInMap(const sge::PixPos& pos) const;
   bool isInDashboard(const sge::PixPos& pos) const;
   bool mapOnLeftButtonPressed(const sge::PixPos& pos);
   bool mapOnLeftButtonReleased(const sge::PixPos& pos);
   bool dashboardOnLeftButtonPressed(const sge::PixPos& pos);
   bool dashboardOnLeftButtonReleased(const sge::PixPos& pos);
   
   // State overrides.
   int credits() const override { return m_credits; }
   bool canAffordDefender(const std::string& model) const override;
   bool isPaused() const override;

   // Commands overrides.
   void startPlaceSession(std::string_view model, std::string_view indicatorTex,
                          sge::PixDim indicatorDim) override;
   void endPlaceSession() override;
   void startAttack() override;
   void pauseAttack() override;
   
   // Convenience functions for coordinate system operations.
   sge::PixPos toPix(sge::MapPos mpos) const;
   sge::MapPos toMap(sge::PixPos ppos) const;
   sge::MapDim scaleInto(sge::MapDim source, sge::MapDim dest) const;

private:
   static constexpr sge::PixCoordi MapWidth = UiScale(1800);
   static constexpr sge::PixCoordi MapHeight = UiScale(1200);
   static constexpr sge::PixCoordi DashboardWidth = UiScale(200);
   static constexpr sge::PixCoordi DashboardHeight = MapHeight;
   static constexpr sge::PixCoordi WndWidth = MapWidth + DashboardWidth;
   static constexpr sge::PixCoordi WndHeight = MapHeight;

   std::unique_ptr<MapCoordSys> m_coordSys;
   Paths m_paths;
   std::unique_ptr<HpRenderer> m_hpRenderer;
   std::unique_ptr<AttackerFactory> m_attackFactory;
   AttackerMap m_attackers;
   std::unique_ptr<DefenderFactory> m_defenseFactory;
   std::vector<Defender> m_defenders;
   std::unique_ptr<Map> m_map;
   sge::Sprite m_background;
   Dashboard m_dashboard;
   std::optional<PlaceSession> m_placeSess;
   sge::Sprite m_invalidFieldOverlay;
   sge::Sprite m_rangeOverlay;
   std::vector<bool> m_defenderMatrix;
   int m_credits = 150;
};


inline sge::PixPos Towers::toPix(sge::MapPos mpos) const
{
   return m_coordSys->toPix(mpos);
}

inline sge::MapPos Towers::toMap(sge::PixPos ppos) const
{
   return m_coordSys->toMap(ppos);
}

inline sge::MapDim Towers::scaleInto(sge::MapDim source, sge::MapDim dest) const
{
   return m_coordSys->scaleInto(source, dest);
}
