//
// Oct-2020, Michael Lindner
// MIT license
//
#pragma once
#include "input_controller.h"
#include "observed.h"
#include "glm/vec2.hpp"

namespace glfwl
{
class Window;
}


///////////////////

class InputState : public InputController, public Observed<InputState>
{
 public:
   glm::vec2 mousePosition() const { return m_mousePos; }
   void pollInput(glfwl::Window& wnd, float elapsedTime);

 private:
   // InputController overrides.
   void onMouseMoved(double xpos, double ypos) override;
   void onMouseScrolled(double xoffset, double yoffset) override;
   void onKeyChanged(int key, int scancode, int action, int mods) override;

   void notifyKeyPolled(int key, float elapsedTime);

 private:
   static constexpr float MouseSensitivity = 0.1f;
   static constexpr float ScrollSensitivity = 2.0f;
   bool m_isFirstMouseMove = true;
   glm::vec2 m_mousePos{0.0, 0.0};
};


///////////////////

// Notifications sent to input state observers.

constexpr char MouseMovedMsg[] = "mouse-moved";
struct MouseMovedMsgData : public Observed<InputState>::MsgData
{
   glm::vec2 pos;
   glm::vec2 rawDelta;
   glm::vec2 adjustedDelta;
};

constexpr char MouseScrolledMsg[] = "mouse-scrolled";
struct MouseScrolledMsgData : public Observed<InputState>::MsgData
{
   glm::vec2 rawDelta;
   glm::vec2 adjustedDelta;
};

constexpr char KeyChangedMsg[] = "key-changed";
struct KeyChangedMsgData : public Observed<InputState>::MsgData
{
   // Glfw key code: GLFW_KEY_SPACE, GLFW_KEY_A, ...
   int key = 0;
   // System-specific scancode.
   int scancode = 0;
   // Key action: GLFW_PRESS, GLFW_RELEASE, GLFW_REPEAT
   int action = 0;
   // Modifier key flags: GLFW_MOD_SHIFT, GLFW_MOD_CONTROL, ...
   int mods = 0;
};

constexpr char KeyPolledMsg[] = "key-polled";
struct KeyPolledMsgData : public Observed<InputState>::MsgData
{
   // Glfw key code: GLFW_KEY_SPACE, GLFW_KEY_A, ...
   int key = 0;
   // Time elapsed since last polling.
   float elapsedTime = 0.0;
};
