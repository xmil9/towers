//
// Feb-2021, Michael Lindner
// MIT license
//
#pragma once
#include "sge_lap_clock.h"


namespace sge
{

using FrameClock = LapClock<float, std::chrono::milliseconds>;
constexpr float MsToSecs = 0.001f;

} // namespace sge
