//
// Oct-2020, Michael Lindner
// MIT license
//
#include "app_window.h"
#include "input_controller.h"
#include "ui_controller.h"
#include <cassert>


void AppWindow::onWindowResized(int width, int height)
{
   Window::onWindowResized(width, height);

   assert(m_uiController);
   if (m_uiController)
      m_uiController->onWindowResized(width, height);
}


void AppWindow::onWindowMouseMoved(double xpos, double ypos)
{
   assert(m_inputController);
   if (m_inputController)
      m_inputController->onMouseMoved(xpos, ypos);
}


void AppWindow::onWindowScrolled(double xoffset, double yoffset)
{
   assert(m_inputController);
   if (m_inputController)
      m_inputController->onMouseScrolled(xoffset, yoffset);
}


void AppWindow::onWindowKeyChanged(Key_t key, int scancode, int action, int mods)
{
   assert(m_inputController);
   if (m_inputController)
      m_inputController->onKeyChanged(key, scancode, action, mods);

   if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
      setShouldClose(true);
}
