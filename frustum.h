//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "app_types.h"


// Represents the section of view space that is visible.
// Content within this space will be rendered. Content outside will be clipped off.
// Defines the clip space of the coordinate transformation sequence:
//   local space -> world space -> view/camera space -> CLIP SPACE -> screen space
class Frustum
{
 public:
 private:
   // The horizontal field-of-view angle. Defines how wide the frustum is.
   Angle_t m_horzFov;
   // Aspect ratio of horizontal to vertical size of the frustum.
   float m_aspect;
   // Coordinates (along the central axis) of the near and far planes that cut off the
   // frustum in the front and back.
   float m_nearPlane;
   float m_farPlane;
};
