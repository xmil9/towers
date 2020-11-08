//
// Oct-2020, Michael Lindner
// MIT license
//
#pragma once
#include "lap_clock.h"
#include "essentutils/angle.h"
#include "glfwl_types.h"


using Fp_t = float;
using Angle_t = sutil::Angle<Fp_t>;

using FrameClock = LapClock<float, std::chrono::milliseconds>;
constexpr float MsToSecs = 0.001f;

using Key_t = glfwl::Key;

enum class DirectionXZ
{
   Left,
   Right,
   Forward,
   Backward
};

