//
// Oct-2020, Michael Lindner
// MIT license
//
#pragma once
#include "input_controller.h"
#include "observed.h"
#include "glm/vec2.hpp"

namespace gfl
{
class Window;
}


///////////////////

class Input : public InputController, public Observed<Input>
{
 public:
   glm::vec2 mousePosition() const { return m_mousePos; }
   void process(gfl::Window& wnd, float frameLengthSecs);

 private:
   // InputController overrides.
   void onMouseMoved(double xpos, double ypos) override;
   void onMouseScrolled(double xoffset, double yoffset) override;
   void onMouseButtonChanged(gfl::MouseButton button, int action, int mods) override;
   void onKeyChanged(gfl::Key key, int scancode, int action, int mods) override;

   void pollKeys(gfl::Window& wnd, float frameLengthSecs);
   void notifyKeyPolled(gfl::Key key, float frameLengthSecs);

 private:
   bool m_isFirstMouseMove = true;
   glm::vec2 m_mousePos{0.0, 0.0};
};


///////////////////

// Notifications sent to input state observers.

constexpr char MouseMovedMsg[] = "mouse-moved";
struct MouseMovedMsgData : public Observed<Input>::MsgData
{
   glm::vec2 pos;
   glm::vec2 delta;
};

constexpr char MouseScrolledMsg[] = "mouse-scrolled";
struct MouseScrolledMsgData : public Observed<Input>::MsgData
{
   glm::vec2 delta;
};

constexpr char MouseButtonChangedMsg[] = "mouse-button-changed";
struct MouseButtonChangedMsgData : public Observed<Input>::MsgData
{
   // Button id: GLFW_MOUSE_BUTTON_1, GLFW_MOUSE_BUTTON_2, etc
   gfl::MouseButton button;
   // Key action: GLFW_PRESS or GLFW_RELEASE
   int action;
   // Modifier key flags: GLFW_MOD_SHIFT, GLFW_MOD_CONTROL, ...
   int mods;
};

constexpr char KeyChangedMsg[] = "key-changed";
struct KeyChangedMsgData : public Observed<Input>::MsgData
{
   // Glfw key code: GLFW_KEY_SPACE, GLFW_KEY_A, ...
   gfl::Key key = 0;
   // System-specific scancode.
   int scancode = 0;
   // Key action: GLFW_PRESS, GLFW_RELEASE, GLFW_REPEAT
   int action = 0;
   // Modifier key flags: GLFW_MOD_SHIFT, GLFW_MOD_CONTROL, ...
   int mods = 0;
};

constexpr char KeyPolledMsg[] = "key-polled";
struct KeyPolledMsgData : public Observed<Input>::MsgData
{
   // Glfw key code: GLFW_KEY_SPACE, GLFW_KEY_A, ...
   gfl::Key key = 0;
   // Frame length in seconds.
   float frameLengthSecs = 0.0;
};
