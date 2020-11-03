//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "app_types.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/mat4x4.hpp"
#include <string>

class InputState;


// Represents the section of view space that is visible.
// Content within this space will be rendered. Content outside will be clipped off.
// Defines the clip space of the coordinate transformation sequence:
//   local space -> world space -> view/camera space -> CLIP SPACE -> screen space
class Frustum
{
 public:
   void setupInput(InputState& input);
   glm::mat4x4 projectionMatrix() const;

private:
   void onInputChanged(InputState& input, std::string_view msg);
   void updateFov(float offset);

 private:
   // The horizontal field-of-view angle. Defines how wide the frustum is.
   Angle_t m_horzFov = Angle_t::makeDegrees(45.0f);
   // Aspect ratio of horizontal to vertical size of the frustum.
   float m_aspect = 1.0f;
   // Coordinates (along the central axis) of the near and far planes that cut off the
   // frustum in the front and back.
   float m_nearPlane = 0.1f;
   float m_farPlane = 100.0f;
};


inline glm::mat4x4 Frustum::projectionMatrix() const
{
   return glm::perspective(m_horzFov.radians(), m_aspect, m_nearPlane, m_farPlane);
}
