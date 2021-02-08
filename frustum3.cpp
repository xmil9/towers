//
// Nov-2020, Michael Lindner
// MIT license
//
#include "frustum3.h"


namespace
{
///////////////////

constexpr sge::Angle_t MaxFov{sge::Angle_t::fromDegrees(60.0f)};
constexpr sge::Angle_t MinFov{sge::Angle_t::fromDegrees(20.0f)};

} // namespace


///////////////////

void Frustum3::updateFov(float offset)
{
   m_horzFov -= esl::radians(offset);

   if (m_horzFov > MaxFov)
      m_horzFov = MaxFov;
   if (m_horzFov < MinFov)
      m_horzFov = MinFov;
}
