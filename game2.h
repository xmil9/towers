//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "attacker.h"
#include "basic_types.h"
#include "glfwl_lib.h"
#include "input.h"
#include "main_window.h"
#include "map.h"
#include "map_coord_sys.h"
#include "renderer2.h"
#include "resources.h"
#include <memory>
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
   bool setupResources();
   bool setupRenderer();
   bool setupTerrain();
   bool setupSpriteData();
   bool setupAttackers();
   bool setupBackground();
   
   void processInput();
   void updateState();
   void render();

   void onMainWindowChanged(MainWindow& src, std::string_view msg,
                            const Observed<MainWindow>::MsgData& data);
   void onMainWindowResize(const glm::ivec2& newSize);

   void onInputChanged(Input& src, std::string_view msg,
                       const Observed<Input>::MsgData& data);
   void onMouseMoved(const glm::vec2& delta);
   void onMouseScrolled(const glm::vec2& delta);
   void onKeyPolled(glfwl::Key key, float frameLengthSecs);

 private:
   static constexpr int MainWndWidth = 1800;
   static constexpr int MainWndHeight = 1200;
   Resources m_resources;
   FrameClock m_frameClock;
   glfwl::Lib m_glfw;
   MainWindow m_mainWnd;
   Input m_input;
   std::unique_ptr<MapCoordSys> m_coordSys;
   Renderer2 m_renderer;
   std::unique_ptr<SpriteRenderer> m_stdSpriteRenderer;
   std::vector<Attacker> m_attackers;
   std::unique_ptr<Map> m_map;
   std::unique_ptr<Sprite> m_background;
};
