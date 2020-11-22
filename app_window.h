//
// Oct-2020, Michael Lindner
// MIT license
//
#pragma once
#include "basic_types.h"
#include "input_controller.h"
#include "observed.h"
#include "glfwl_window.h"
#include "glm/vec2.hpp"



class AppWindow : public glfwl::Window, public Observed<AppWindow>
{
public:
   void setInputController(InputController* controller);

 protected:
   void onWindowResized(int width, int height) override;
   void onWindowMouseMoved(double xpos, double ypos) override;
   void onWindowScrolled(double xoffset, double yoffset) override;
   void onWindowKeyChanged(Key_t key, int scancode, int action, int mods) override;

private:
   InputController* m_inputController = nullptr;
   glm::ivec2 m_wndSize;
};


///////////////////

// Notifications sent to observers.

constexpr char WindowResizedMsg[] = "window_resized";
struct WindowResizedMsgData : public Observed<AppWindow>::MsgData
{
   glm::ivec2 newSize;
   // Difference to previous size.
   glm::ivec2 diff;
};
