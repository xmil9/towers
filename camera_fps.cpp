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
   else if (msg == KeyChangedMsg)
   {
      const auto keyData = static_cast<const KeyChangedMsgData&>(data);
      processKeyChange(keyData.key, keyData.action);
   }
}


void CameraFps::processKeyChange(int key, int action)
{
   switch (key)
   {
   case GLFW_KEY_W:
      if (action == GLFW_PRESS)
         updateCameraPosition(front());
      return;
   case GLFW_KEY_S:
      if (action == GLFW_PRESS)
         updateCameraPosition(back());
      return;
   case GLFW_KEY_A:
      if (action == GLFW_PRESS)
         updateCameraPosition(left());
      return;
   case GLFW_KEY_D:
      if (action == GLFW_PRESS)
         updateCameraPosition(right());
      return;
   }
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
