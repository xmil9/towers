//
// Nov-2020, Michael Lindner
// MIT license
//
#include "sge_frustum3.h"


namespace
{
///////////////////

constexpr sge::Angle MaxFov{sge::Angle::fromDegrees(60.0f)};
constexpr sge::Angle MinFov{sge::Angle::fromDegrees(20.0f)};

} // namespace


namespace sge
{
///////////////////

void Frustum3::updateFov(float offset)
{
   m_horzFov -= esl::radians(offset);

   if (m_horzFov > MaxFov)
      m_horzFov = MaxFov;
   if (m_horzFov < MinFov)
      m_horzFov = MinFov;
}

} // namespace sge
