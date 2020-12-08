//
// Oct-2020, Michael Lindner
// MIT license
//
#include "input.h"
#include "gfl_window.h"
#include <array>


void Input::process(gfl::Window& wnd, float frameLengthSecs)
{
   wnd.pollEvents();
   pollKeys(wnd, frameLengthSecs);
}


void Input::onMouseMoved(double xpos, double ypos)
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
      data.delta = delta;
      notify(*this, MouseMovedMsg, data);
   }
}


void Input::onMouseScrolled(double xoffset, double yoffset)
{
   const glm::vec2 delta(xoffset, yoffset);
   MouseScrolledMsgData data;
   data.delta = delta;
   notify(*this, MouseScrolledMsg, data);
}


void Input::onKeyChanged(gfl::Key key, int scancode, int action, int mods)
{
   KeyChangedMsgData data;
   data.key = key;
   data.scancode = scancode;
   data.action = action;
   data.mods = mods;
   notify(*this, KeyChangedMsg, data);
}


void Input::pollKeys(gfl::Window& wnd, float frameLengthSecs)
{
   static constexpr std::array<int, 4> PolledKeys = {GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_A,
                                                     GLFW_KEY_D};

   for (auto key : PolledKeys)
   {
      if (glfwGetKey(wnd.handle(), key) == GLFW_PRESS)
         notifyKeyPolled(key, frameLengthSecs);
   }
}


void Input::notifyKeyPolled(gfl::Key key, float frameLengthSecs)
{
   KeyPolledMsgData data;
   data.key = key;
   data.frameLengthSecs = frameLengthSecs;
   notify(*this, KeyPolledMsg, data);
}
