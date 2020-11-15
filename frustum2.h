//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "glad/glad.h" // glad must be included before anything else opengl related.
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
};


inline glm::mat4x4 Frustum2::projectionMatrix() const
{
   return glm::ortho(0, m_width, 0, m_height);
}


inline void Frustum2::setSize(int width, int height)
{
   m_width = width;
   m_height = height;
}
