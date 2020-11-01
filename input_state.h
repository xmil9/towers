//
// Oct-2020, Michael Lindner
// MIT license
//
#pragma once
#include "app_window.h"
#include "input_controller.h"
#include "observed.h"
#include "glm/vec2.hpp"


constexpr char MouseMovedMsg[] = "mouse-moved";
constexpr char MouseScrolledMsg[] = "mouse-scrolled";


class InputState : public InputController, public Observed<InputState>
{
 public:
   glm::vec2 mousePosition() const { return m_mousePos; }
   glm::vec2 rawMouseDelta() const { return m_mouseDelta; }
   glm::vec2 adjMouseDelta() const { return m_mouseDelta * MouseSensitivity; }

 private:
   // InputController overrides.
   void onMouseMoved(double xpos, double ypos) override;
   void onMouseScrolled(double xoffset, double yoffset) override;

 private:
   static constexpr float MouseSensitivity = 0.1f;
   bool m_isFirstMouseMove = true;
   glm::vec2 m_mousePos{0.0, 0.0};
   glm::vec2 m_mouseDelta{0.0, 0.0};
};
