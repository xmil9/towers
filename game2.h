//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "basic_types.h"
#include "glfwl_lib.h"
#include "input.h"
#include "main_window.h"
#include "renderer2.h"
#include "resources.h"
#include "sprite.h"
#include "terrain.h"
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
   bool setupSpriteData();
   bool setupTerrain();

   void onMainWindowChanged(MainWindow& src, std::string_view msg,
                            const Observed<MainWindow>::MsgData& data);
   void onMainWindowResize(const glm::ivec2& newSize);

   void onInputChanged(Input& src, std::string_view msg,
                       const Observed<Input>::MsgData& data);
   void onMouseMoved(const glm::vec2& delta);
   void onMouseScrolled(const glm::vec2& delta);
   void onKeyPolled(Key_t key, float frameLengthSecs);

 private:
   static constexpr int MainWndWidth = 900;
   static constexpr int MainWndHeight = 600;
   Resources m_resources;
   FrameClock m_frameClock;
   glfwl::Lib m_glfw;
   MainWindow m_mainWnd;
   Input m_input;
   Renderer2 m_renderer;
   std::vector<Sprite> m_sprites;
   Terrain m_terrain;
};
