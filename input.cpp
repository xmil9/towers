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
      MouseMovedData data;
      data.pos = m_mousePos;
      data.delta = delta;
      notify(*this, MouseMovedEvent, data);
   }
}


void Input::onMouseScrolled(double xoffset, double yoffset)
{
   const glm::vec2 delta(xoffset, yoffset);
   MouseScrolledData data;
   data.delta = delta;
   notify(*this, MouseScrolledEvent, data);
}


void Input::onMouseButtonChanged(gfl::MouseButton button, int action, int mods,
                                 double xpos, double ypos)
{
   MouseButtonChangedData data;
   data.button = button;
   data.action = action;
   data.mods = mods;
   data.pos = {xpos, ypos};
   notify(*this, MouseButtonChangedEvent, data);
}


void Input::onKeyChanged(gfl::Key key, int scancode, int action, int mods)
{
   KeyChangedData data;
   data.key = key;
   data.scancode = scancode;
   data.action = action;
   data.mods = mods;
   notify(*this, KeyChangedEvent, data);
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
   KeyPolledData data;
   data.key = key;
   data.frameLengthSecs = frameLengthSecs;
   notify(*this, KeyPolledEvent, data);
}
