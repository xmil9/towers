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

   const glm::vec2 delta = newPos - m_mousePos;
   m_mousePos = newPos;

   if (delta.x != 0.0 || delta.y != 0.0)
   {
      MouseMovedMsgData data;
      data.pos = m_mousePos;
      data.rawDelta = delta;
      data.adjustedDelta = delta * MouseSensitivity;
      notify(*this, MouseMovedMsg, data);
   }
}


void InputState::onMouseScrolled(double xoffset, double yoffset)
{
   const glm::vec2 delta(xoffset, yoffset);
   MouseScrolledMsgData data;
   data.rawDelta = delta;
   data.adjustedDelta = delta * ScrollSensitivity;
   notify(*this, MouseScrolledMsg, data);
}


void InputState::onKeyChanged(int key, int scancode, int action, int mods)
{
   KeyChangedMsgData data;
   data.key = key;
   data.scancode = scancode;
   data.action = action;
   data.mods = mods;
   notify(*this, KeyChangedMsg, data);
}
