//
// Nov-2020, Michael Lindner
// MIT license
//
#include "sge_camera_fps.h"
#include "essentutils/math_util.h"
#include <cassert>
#include <cmath>


namespace
{
///////////////////

constexpr sge::Angle MaxPitch{sge::Angle::fromDegrees(89.0f)};
constexpr sge::Angle MinPitch{sge::Angle::fromDegrees(-89.0f)};

} // namespace


namespace sge
{
///////////////////

void CameraFps::updateDirection(const glm::vec2& offset)
{
   m_yaw += esl::radians(offset.x);
   m_pitch += esl::radians(offset.y);

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


glm::vec3 CameraFps::directionVector(DirectionXZ dir) const
{
   switch (dir)
   {
   case DirectionXZ::Left:
      return left();
   case DirectionXZ::Right:
      return right();
   case DirectionXZ::Forward:
      return front();
   case DirectionXZ::Backward:
      return back();
   }

   assert(false && "Unexpected direction.");
   return {};
}

} // namespace sge
