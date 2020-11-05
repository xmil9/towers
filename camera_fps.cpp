//
// Nov-2020, Michael Lindner
// MIT license
//
#include "camera_fps.h"
#include "input_state.h"
#include "essentutils/math_util.h"
#include "glfw/glfw3.h"
#include <cassert>
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
   input.addObserver([this](InputState& input, std::string_view msg,
                            const Observed<InputState>::MsgData& data) {
      onInputChanged(input, msg, data);
   });
}


void CameraFps::onInputChanged(InputState& /*input*/, std::string_view msg,
                               const Observed<InputState>::MsgData& data)
{
   if (msg == MouseMovedMsg)
   {
      const auto mouseMovedData = static_cast<const MouseMovedMsgData&>(data);
      updateCameraDirection(mouseMovedData.adjustedDelta);
   }
   else if (msg == KeyPolledMsg)
   {
      const auto keyData = static_cast<const KeyPolledMsgData&>(data);
      processKeyPoll(keyData.key, keyData.elapsedTime);
   }
}


void CameraFps::processKeyPoll(int key, float elapsedTime)
{
   const auto movementDir = directionForKey(key);
   if (movementDir)
      updateCameraPosition(*movementDir * elapsedTime);

   // Process non-movement keys if necessary.
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


void CameraFps::updateCameraPosition(const glm::vec3& offset)
{
   m_eye += offset * MovementSpeed;
}


std::optional<glm::vec3> CameraFps::directionForKey(int key)
{
   switch (key)
   {
   case GLFW_KEY_W:
      return front();
   case GLFW_KEY_S:
      return back();
   case GLFW_KEY_A:
      return left();
   case GLFW_KEY_D:
      return right();
   default:
      return std::nullopt;
   }
}
