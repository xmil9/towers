//
// Oct-2020, Michael Lindner
// MIT license
//
#pragma once
#include "input_controller.h"
#include "glfwl_window.h"
#include <vector>


class AppWindow : public glfwl::Window
{
 public:
   void setInputController(InputController* input) { m_inputController = input; }

 protected:
   void onWindowMouseMoved(double xpos, double ypos) override;
   void onWindowScrolled(double xoffset, double yoffset) override;
   void onWindowKeyChanged(int key, int scancode, int action, int mods) override;

 private:
   InputController* m_inputController;
};
