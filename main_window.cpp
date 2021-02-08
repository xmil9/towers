//
// Oct-2020, Michael Lindner
// MIT license
//
#include "main_window.h"
#include <cassert>
#include <limits>


void MainWindow::setInputController(sge::InputController* controller)
{
   assert(controller);
   m_inputController = controller;
}


sge::PixPos MainWindow::mousePosition() const
{
   const auto mousePos = cursorPosition();
   if (!mousePos)
      return sge::PixPos{-1.f, -1.f};

   return {static_cast<sge::PixCoord>(mousePos->first),
           static_cast<sge::PixCoord>(mousePos->second)};
}


void MainWindow::onWindowResized(int width, int height)
{
   Window::onWindowResized(width, height);

   const glm::ivec2 newSize(width, height);
   const glm::ivec2 diff = newSize - m_wndSize;
   m_wndSize = newSize;

   if (diff.x != 0 || diff.y != 0)
   {
      WindowResizedData data;
      data.newSize = newSize;
      data.diff = diff;
      notify(*this, WindowResizedEvent, data);
   }
}


void MainWindow::onWindowMouseMoved(double xpos, double ypos)
{
   assert(m_inputController);
   if (m_inputController)
      m_inputController->onMouseMoved(xpos, ypos);
}


void MainWindow::onWindowScrolled(double xoffset, double yoffset)
{
   assert(m_inputController);
   if (m_inputController)
      m_inputController->onMouseScrolled(xoffset, yoffset);
}


void MainWindow::onWindowKeyChanged(gfl::Key key, int scancode, int action, int mods)
{
   assert(m_inputController);
   if (m_inputController)
      m_inputController->onKeyChanged(key, scancode, action, mods);

   if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
      setShouldClose(true);
}


void MainWindow::onWindowMouseButtonChanged(gfl::MouseButton button, int action, int mods,
                                            double xpos, double ypos)
{
   assert(m_inputController);
   if (m_inputController)
      m_inputController->onMouseButtonChanged(button, action, mods, xpos, ypos);
}
