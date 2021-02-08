//
// Oct-2020, Michael Lindner
// MIT license
//
#pragma once
#include <cstdint>


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

using EntityId = int64_t;
