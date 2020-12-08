//
// Oct-2020, Michael Lindner
// MIT license
//
#pragma once
#include "lap_clock.h"


using FrameClock = LapClock<float, std::chrono::milliseconds>;
constexpr float MsToSecs = 0.001f;

enum class DirectionXZ
{
   Left,
   Right,
   Forward,
   Backward
};

