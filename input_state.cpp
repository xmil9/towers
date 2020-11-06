//
// Oct-2020, Michael Lindner
// MIT license
//
#include "input_state.h"
#include "glfwl_window.h"
#include <array>


void InputState::pollInput(glfwl::Window& wnd, float frameLengthSecs)
{
   static constexpr std::array<int, 4> PolledKeys = {
      GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_A, GLFW_KEY_D
   };

   for (auto key : PolledKeys)
   {
      if (glfwGetKey(wnd.handle(), key) == GLFW_PRESS)
         notifyKeyPolled(key, frameLengthSecs);
   }
}


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


void InputState::notifyKeyPolled(int key, float frameLengthSecs)
{
   KeyPolledMsgData data;
   data.key = key;
   data.frameLengthSecs = frameLengthSecs;
   notify(*this, KeyPolledMsg, data);
}
