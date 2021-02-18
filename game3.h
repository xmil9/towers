//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "renderer3.h"
#include "spiel/direction.h"
#include "spiel/input.h"
#include "spiel/lap_clock.h"
#include "spiel/main_window.h"
#include "glm/vec3.hpp"
#include "opengl_util/gfl_lib.h"
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

   void onMainWindowChanged(sp::MainWindow& src, std::string_view event,
                            const esl::ObservedEventData& data);
   void onMainWindowResize(const glm::ivec2& newSize);

   void onInputChanged(sp::Input& src, std::string_view event,
                       const esl::ObservedEventData& data);
   void onMouseMoved(const glm::vec2& delta);
   void onMouseScrolled(const glm::vec2& delta);
   void onKeyPolled(gfl::Key key, float frameLengthSecs);

   std::optional<sp::DirectionXZ> getCameraDirection(gfl::Key key) const;
   void updateCameraPosition(gfl::Key key, float frameLengthSecs);

 private:
   using FrameClock = sp::LapClock<float, std::chrono::milliseconds>;
   
   FrameClock m_frameClock;
   gfl::Lib m_glfw;
   sp::MainWindow m_mainWnd;
   sp::Input m_input;
   Renderer3 m_renderer;
};
