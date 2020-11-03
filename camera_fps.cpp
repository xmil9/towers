//
// Nov-2020, Michael Lindner
// MIT license
//
#include "camera_fps.h"
#include "input_state.h"
#include "essentutils/math_util.h"
#include <cmath>


namespace
{
///////////////////

constexpr Angle_t MaxPitch{Angle_t::fromDegrees(89.0f)};
constexpr Angle_t MinPitch{Angle_t::fromDegrees(-89.0f)};

} // namespace


///////////////////

void CameraFps::setupInput(InputState& input)
{
   input.addObserver(
      [this](InputState& input, std::string_view msg) { onInputChanged(input, msg); });
}


void CameraFps::onInputChanged(InputState& input, std::string_view msg)
{
   if (msg == MouseMovedMsg)
      updateCameraDirection(input.adjMouseDelta());
}


void CameraFps::updateCameraDirection(const glm::vec2& offset)
{
   m_yaw += sutil::radians(offset.x);
   m_pitch += sutil::radians(offset.y);

   if (m_pitch > MaxPitch)
      m_pitch = MaxPitch;
   if (m_pitch < MinPitch)
      m_pitch = MinPitch;

   glm::vec3 dir;
   dir.x = std::cos(m_yaw) * std::cos(m_pitch);
   dir.y = std::sin(m_pitch);
   dir.z = std::sin(m_yaw) * std::cos(m_pitch);
   m_direction = glm::normalize(dir);
}
