//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "attacker.h"
#include "attacker_factoy.h"
#include "basic_types.h"
#include "coords.h"
#include "defender.h"
#include "defender_factory.h"
#include "gfl_lib.h"
#include "input.h"
#include "main_window.h"
#include "map.h"
#include "map_coord_sys.h"
#include "renderer2.h"
#include "resources.h"
#include <memory>
#include <optional>
#include <vector>


class Game2
{
 public:
   Game2();

   bool setup();
   void cleanup();
   void run();

 private:
   bool setupUi();
   bool setupMainWindow();
   bool setupInput();
   bool setupOutput();
   bool setupTextures();
   bool setupRenderer();
   bool setupTerrain();
   bool setupSpriteData();
   bool setupAnimations();
   bool setupAttackers();
   bool setupDefenders();
   bool setupBackground();
   bool setupDashboard();

   void processInput();
   void updateState();
   void render();
   void renderMap();
   void renderDashboard();
   void renderLocationSession();

   void onMainWindowChanged(MainWindow& src, std::string_view msg,
                            const Observed<MainWindow>::MsgData& data);
   void onMainWindowResize(const glm::ivec2& newSize);

   void onInputChanged(Input& src, std::string_view msg,
                       const Observed<Input>::MsgData& data);
   void onMouseMoved(const glm::vec2& delta);
   void onMouseScrolled(const glm::vec2& delta);
   void onMouseButtonChanged(gfl::MouseButton button, int action, const glm::vec2& pos);
   void onLeftButtonPressed(const glm::vec2& pos);
   void onLeftButtonReleased(const glm::vec2& pos);
   void onRightButtonPressed(const glm::vec2& pos);
   void onRightButtonReleased(const glm::vec2& pos);
   void onKeyPolled(gfl::Key key, float frameLengthSecs);

   bool mapOnLeftButtonPressed(const glm::vec2& pos);
   bool mapOnLeftButtonReleased(const glm::vec2& pos);
   bool dashboardOnLeftButtonPressed(const glm::vec2& pos);
   bool dashboardOnLeftButtonReleased(const glm::vec2& pos);

 private:
   static constexpr PixCoordi MapWidth = 1800;
   static constexpr PixCoordi MapHeight = 1200;
   static constexpr PixCoordi DashboardWidth = 200;
   static constexpr PixCoordi DashboardHeight = MapHeight;
   static constexpr PixCoordi WndWidth = MapWidth + DashboardWidth;
   static constexpr PixCoordi WndHeight = MapHeight;

   Resources m_resources;
   FrameClock m_frameClock;
   gfl::Lib m_glfw;
   MainWindow m_mainWnd;
   Input m_input;
   std::unique_ptr<MapCoordSys> m_coordSys;
   Renderer2 m_renderer;
   std::unique_ptr<SpriteRenderer> m_spriteRenderer;
   std::unique_ptr<AttackerFactory> m_attackFactory;
   std::vector<Attacker> m_attackers;
   std::unique_ptr<DefenderFactory> m_defenseFactory;
   std::vector<Defender> m_defenders;
   std::unique_ptr<Map> m_map;
   std::unique_ptr<Sprite> m_background;
   std::unique_ptr<Sprite> m_dashboard;
   std::unique_ptr<Sprite> m_ltButton;

   struct PlaceSession
   {
      std::unique_ptr<Sprite> indicator;
   };
   std::optional<PlaceSession> m_placeSess;
};
