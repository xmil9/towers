//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "glad/glad.h" // glad must be included before anything else opengl related.
#include "glm/gtc/matrix_transform.hpp"
#include "glm/mat4x4.hpp"
#include <string>


namespace sp
{
///////////////////

// Represents the section of view/camera space that is visible.
// For a 2D frustum this is an orthographic (no perspective) view.
class Frustum2
{
 public:
   // Matrix to transform view coords to clip space (frustum) coords.
   glm::mat4x4 projectionMatrix() const;
   void setSize(int width, int height);

 private:
   float m_width = 0.f;
   float m_height = 0.f;
};


inline glm::mat4x4 Frustum2::projectionMatrix() const
{
   // Flip y-axis so that (0, 0) is at left-top.
   return glm::ortho(0.f, m_width, m_height, 0.f, -1.f, 1.f);
}


inline void Frustum2::setSize(int width, int height)
{
   m_width = static_cast<float>(width);
   m_height = static_cast<float>(height);
}

} // namespace sp
