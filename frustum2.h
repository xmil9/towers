//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "glm/gtc/matrix_transform.hpp"
#include "glm/mat4x4.hpp"
#include <string>


// Represents the section of view/camera space that is visible.
// For a 2D frustum this is an orthographic (no perspective) view.
class Frustum2
{
 public:
   // Matrix to transform view coords to clip space (frustum) coords.
   glm::mat4x4 projectionMatrix() const;
   void setSize(int width, int height);

 private:
   int m_width = 0;
   int m_height = 0;
   // Coordinates (along the central axis) of the near and far planes that cut off the
   // frustum in the front and back.
   static constexpr float m_nearPlane = 1.0f;
   static constexpr float m_farPlane = -1.0f;
};


inline glm::mat4x4 Frustum2::projectionMatrix() const
{
   return glm::ortho(0, m_width, 0, m_height, m_nearPlane, m_farPlane);
}


inline void Frustum2::setSize(int width, int height)
{
   m_width = width;
   m_height = height;
}
