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

template<typename T>
constexpr T UiScale(T val)
{
   return val * T(1);//T(2) / T(3);
}
