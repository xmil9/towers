//
// Oct-2020, Michael Lindner
// MIT license
//
#pragma once
#include "input_controller.h"
#include "essentutils/observed.h"
#include "glm/vec2.hpp"

namespace gfl
{
class Window;
}


///////////////////

class Input : public InputController, public sutil::Observed<Input>
{
 public:
   glm::vec2 mousePosition() const { return m_mousePos; }
   void process(gfl::Window& wnd, float frameLengthSecs);

 private:
   // InputController overrides.
   void onMouseMoved(double xpos, double ypos) override;
   void onMouseScrolled(double xoffset, double yoffset) override;
   void onMouseButtonChanged(gfl::MouseButton button, int action, int mods, double xpos,
                             double ypos) override;
   void onKeyChanged(gfl::Key key, int scancode, int action, int mods) override;

   void pollKeys(gfl::Window& wnd, float frameLengthSecs);
   void notifyKeyPolled(gfl::Key key, float frameLengthSecs);

 private:
   bool m_isFirstMouseMove = true;
   glm::vec2 m_mousePos{0.f, 0.f};
};


///////////////////

// Notifications sent to input state observers.

constexpr char MouseMovedEvent[] = "mouse-moved";
struct MouseMovedData : public sutil::ObservedEventData
{
   glm::vec2 pos{0.f, 0.f};
   glm::vec2 delta{0.f, 0.f};
};

constexpr char MouseScrolledEvent[] = "mouse-scrolled";
struct MouseScrolledData : public sutil::ObservedEventData
{
   glm::vec2 delta{0.f, 0.f};
};

constexpr char MouseButtonChangedEvent[] = "mouse-button-changed";
struct MouseButtonChangedData : public sutil::ObservedEventData
{
   // Button id: GLFW_MOUSE_BUTTON_1, GLFW_MOUSE_BUTTON_2, etc
   gfl::MouseButton button = 0;
   // Key action: GLFW_PRESS or GLFW_RELEASE
   int action = 0;
   // Modifier key flags: GLFW_MOD_SHIFT, GLFW_MOD_CONTROL, ...
   int mods = 0;
   // Mouse position relative to window.
   glm::vec2 pos{0.f, 0.f};
};

constexpr char KeyChangedEvent[] = "key-changed";
struct KeyChangedData : public sutil::ObservedEventData
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

constexpr char KeyPolledEvent[] = "key-polled";
struct KeyPolledData : public sutil::ObservedEventData
{
   // Glfw key code: GLFW_KEY_SPACE, GLFW_KEY_A, ...
   gfl::Key key = 0;
   // Frame length in seconds.
   float frameLengthSecs = 0.f;
};
