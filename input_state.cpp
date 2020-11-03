//
// Oct-2020, Michael Lindner
// MIT license
//
#include "input_state.h"


void InputState::onMouseMoved(double xpos, double ypos)
{
   const glm::vec2 newPos(xpos, ypos);

   if (m_isFirstMouseMove)
   {
      m_mousePos = newPos;
      m_isFirstMouseMove = false;
   }

   m_mouseDelta = newPos - m_mousePos;
   m_mousePos = newPos;

   if (m_mouseDelta.x != 0.0 || m_mouseDelta.y != 0.0)
      notify(*this, MouseMovedMsg);
}


void InputState::onMouseScrolled(double xoffset, double yoffset)
{
   m_scrollDelta = glm::vec2(xoffset, yoffset);
   notify(*this, MouseScrolledMsg);
}
