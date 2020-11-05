//
// Oct-2020, Michael Lindner
// MIT license
//
#pragma once
#include "app_types.h"
#include "input_state.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/matrix.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include <optional>
#include <string>


class CameraFps
{
 public:
   void setupInput(InputState& input);
   glm::mat4x4 viewMatrix() const;

 private:
   void onInputChanged(InputState& input, std::string_view msg,
                       const Observed<InputState>::MsgData& data);
   void processKeyPoll(int key, float elapsedTime);

   void updateCameraDirection(const glm::vec2& offset);
   void updateCameraPosition(const glm::vec3& offset);

   glm::vec3 front() const { return m_direction; }
   glm::vec3 back() const { return -front(); }
   glm::vec3 left() const { return -right(); }
   glm::vec3 right() const { return glm::normalize(glm::cross(front(), m_up)); }
   std::optional<glm::vec3> directionForKey(int key);

 private:
   static constexpr float MovementSpeed = 2.5f;
   // Current yaw and pitch values of camera. The camera's direction is calculated from
   // the yaw and pitch values.
   Angle_t m_yaw = Angle_t::fromDegrees(-90.0f);
   Angle_t m_pitch = Angle_t::fromDegrees(0.0f);
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
