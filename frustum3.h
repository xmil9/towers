//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "basic_types.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/mat4x4.hpp"
#include <string>


// Represents the section of view space that is visible.
// Content within this space will be rendered. Content outside will be clipped off.
// Defines the clip space of the coordinate transformation sequence:
//   local space -> world space -> view/camera space -> CLIP SPACE -> screen space
class Frustum3
{
 public:
   // Matrix to transform view coords to clip space (frustum) coords.
   // Note: A final viewport transformation will happen from clip space to 2D screen
   // space. The matrix of this transformation is defined by glViewport.
   glm::mat4x4 projectionMatrix() const;
   void setAspect(float aspect) { m_aspect = aspect; }
   void updateFov(float offset);

 private:
   // The horizontal field-of-view angle. Defines how wide the frustum is.
   Angle_t m_horzFov = Angle_t::fromDegrees(45.0f);
   // Aspect ratio of horizontal to vertical size of the frustum.
   float m_aspect = 1.0f;
   // Coordinates (along the central axis) of the near and far planes that cut off the
   // frustum in the front and back.
   float m_nearPlane = 0.1f;
   float m_farPlane = 100.0f;
};


inline glm::mat4x4 Frustum3::projectionMatrix() const
{
   return glm::perspective(m_horzFov.radians(), m_aspect, m_nearPlane, m_farPlane);
}
