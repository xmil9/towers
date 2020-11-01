//
// Oct-2020, Michael Lindner
// MIT license
//
#include "app_window.h"
#include <cassert>


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
