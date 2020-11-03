//
// Nov-2020, Michael Lindner
// MIT license
//
#include "frustum.h"
#include "input_state.h"


namespace
{
///////////////////

constexpr Angle_t MaxFov{Angle_t::makeDegrees(60.0f)};
constexpr Angle_t MinFov{Angle_t::makeDegrees(20.0f)};

} // namespace


///////////////////

void Frustum::setupInput(InputState& input)
{
   input.addObserver(
      [this](InputState& input, std::string_view msg) { onInputChanged(input, msg); });
}


void Frustum::onInputChanged(InputState& input, std::string_view msg)
{
   if (msg == MouseScrolledMsg)
      updateFov(input.adjScrollDelta().y);
}


void Frustum::updateFov(float offset)
{
   m_horzFov -= sutil::radians(offset);

   if (m_horzFov > MaxFov)
      m_horzFov = MaxFov;
   if (m_horzFov < MinFov)
      m_horzFov = MinFov;
}
