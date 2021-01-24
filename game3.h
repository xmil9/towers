//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "basic_types.h"
#include "gfl_lib.h"
#include "input.h"
#include "main_window.h"
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
   bool setupOutput();

   void onMainWindowChanged(MainWindow& src, std::string_view event,
                            const ObservedEventData& data);
   void onMainWindowResize(const glm::ivec2& newSize);

   void onInputChanged(Input& src, std::string_view event, const ObservedEventData& data);
   void onMouseMoved(const glm::vec2& delta);
   void onMouseScrolled(const glm::vec2& delta);
   void onKeyPolled(gfl::Key key, float frameLengthSecs);

   std::optional<DirectionXZ> getCameraDirection(gfl::Key key) const;
   void updateCameraPosition(gfl::Key key, float frameLengthSecs);

 private:
   FrameClock m_frameClock;
   gfl::Lib m_glfw;
   MainWindow m_mainWnd;
   Input m_input;
   Renderer3 m_renderer;
};
