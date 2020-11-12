//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "app_types.h"
#include "app_window.h"
#include "glfwl_lib.h"
#include "input.h"
#include "renderer3.h"
#include "glm/vec3.hpp"
#include <optional>


class Game3
{
 public:
   Game3();

   bool setup();
   void cleanup();
   void run();

 private:
   bool setupUi();
   bool setupMainWindow();
   bool setupInput();

   void onMainWindowChanged(AppWindow& src, std::string_view msg,
                            const Observed<AppWindow>::MsgData& data);
   void onMainWindowResize(const glm::ivec2& newSize);

   void onInputChanged(Input& src, std::string_view msg,
                       const Observed<Input>::MsgData& data);
   void onMouseMoved(const glm::vec2& delta);
   void onMouseScrolled(const glm::vec2& delta);
   void onKeyPolled(Key_t key, float frameLengthSecs);

   std::optional<DirectionXZ> getCameraDirection(Key_t key) const;
   void updateCameraPosition(Key_t key, float frameLengthSecs);

 private:
   FrameClock m_frameClock;
   glfwl::Lib m_glfw;
   AppWindow m_mainWnd;
   Input m_input;
   Renderer3 m_renderer;
};
