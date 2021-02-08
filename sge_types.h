//
// Feb-2021, Michael Lindner
// MIT license
//
#pragma once
#include "sge_lap_clock.h"
#include "glm/vec3.hpp"


namespace sge
{

using FrameClock = LapClock<float, std::chrono::milliseconds>;
constexpr float MsToSecs = 0.001f;

using Color = glm::vec3;
constexpr Color White{1.f, 1.f, 1.f};
constexpr Color Black{0.f, 0.f, 0.f};
constexpr Color NoColor = White;

} // namespace sge
