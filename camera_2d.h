//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "glad/glad.h" // glad must be included before anything else opengl related.
#include "glm/gtc/matrix_transform.hpp"
#include "glm/matrix.hpp"
#include "glm/vec3.hpp"


class Camera2d
{
 public:
   glm::vec3 position() const { return m_eye; }
   // Calculates matrix to transform world coords to camera view coords.
   glm::mat4x4 viewMatrix() const;
   void updatePosition(const glm::vec3& offset);

 private:
   // Location of camera in world space.
   glm::vec3 m_eye = glm::vec3(0.f, 0.f, 0.f);
   // Normalized direction that camera is looking at.
   static constexpr glm::vec3 Direction = glm::vec3(0.f, 0.f, -1.f);
   // Normalized direction that is upwards for camera.
   static constexpr glm::vec3 Up = glm::vec3(0.f, 1.f, 0.f);
};


inline glm::mat4x4 Camera2d::viewMatrix() const
{
   return glm::lookAt(m_eye, m_eye + Direction, Up);
}


inline void Camera2d::updatePosition(const glm::vec3& offset)
{
   m_eye += offset;
}
