//
// Oct-2020, Michael Lindner
// MIT license
//
#pragma once
#include "input_controller.h"
#include "observed.h"
#include "gfl_window.h"
#include "glm/vec2.hpp"


class MainWindow : public gfl::Window, public Observed<MainWindow>
{
 public:
   void setInputController(InputController* controller);

 protected:
   void onWindowResized(int width, int height) override;
   void onWindowMouseMoved(double xpos, double ypos) override;
   void onWindowScrolled(double xoffset, double yoffset) override;
   void onWindowKeyChanged(gfl::Key key, int scancode, int action, int mods) override;

 private:
   InputController* m_inputController = nullptr;
   glm::ivec2 m_wndSize;
};


///////////////////

// Notifications sent to observers.

constexpr char WindowResizedMsg[] = "window_resized";
struct WindowResizedMsgData : public Observed<MainWindow>::MsgData
{
   glm::ivec2 newSize;
   // Difference to previous size.
   glm::ivec2 diff;
};
