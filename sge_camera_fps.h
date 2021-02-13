//
// Oct-2020, Michael Lindner
// MIT license
//
#pragma once
#include "sge_coords.h"
#include "sge_direction.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/matrix.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include <optional>


namespace sge
{
///////////////////

class CameraFps
{
 public:
   glm::vec3 position() const { return m_eye; }
   // Calculates matrix to transform world coords to camera view coords.
   glm::mat4x4 viewMatrix() const;
   void updateDirection(const glm::vec2& offset);
   void updatePosition(DirectionXZ dir, float dist);

 private:
   glm::vec3 front() const { return m_direction; }
   glm::vec3 back() const { return -front(); }
   glm::vec3 left() const { return -right(); }
   glm::vec3 right() const { return glm::normalize(glm::cross(front(), m_up)); }
   glm::vec3 directionVector(DirectionXZ dir) const;

 private:
   // Current yaw and pitch values of camera. The camera's direction is calculated from
   // the yaw and pitch values.
   Angle m_yaw = Angle::fromDegrees(-90.0f);
   Angle m_pitch = Angle::fromDegrees(0.0f);
   // Location of camera in world space.
   glm::vec3 m_eye = glm::vec3(0.0f, 0.0f, 3.0f);
   // Normalized direction that camera is looking at.
   glm::vec3 m_direction = glm::vec3(0.0f, 0.0f, -1.0f);
   // Normalized direction that is upwards for camera.
   glm::vec3 m_up = glm::vec3(0.0f, 1.0f, 0.0f);
};


inline glm::mat4x4 CameraFps::viewMatrix() const
{
   return glm::lookAt(m_eye, m_eye + m_direction, m_up);
}


inline void CameraFps::updatePosition(DirectionXZ dir, float dist)
{
   m_eye += directionVector(dir) * dist;
}

} // namespace sge
