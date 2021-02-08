//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "basic_types.h"
#include "renderer3.h"
#include "sge_input.h"
#include "sge_main_window.h"
#include "sge_types.h"
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

   void onMainWindowChanged(sge::MainWindow& src, std::string_view event,
                            const esl::ObservedEventData& data);
   void onMainWindowResize(const glm::ivec2& newSize);

   void onInputChanged(sge::Input& src, std::string_view event,
                       const esl::ObservedEventData& data);
   void onMouseMoved(const glm::vec2& delta);
   void onMouseScrolled(const glm::vec2& delta);
   void onKeyPolled(gfl::Key key, float frameLengthSecs);

   std::optional<DirectionXZ> getCameraDirection(gfl::Key key) const;
   void updateCameraPosition(gfl::Key key, float frameLengthSecs);

 private:
   sge::FrameClock m_frameClock;
   gfl::Lib m_glfw;
   sge::MainWindow m_mainWnd;
   sge::Input m_input;
   Renderer3 m_renderer;
};
