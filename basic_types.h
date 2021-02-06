//
// Oct-2020, Michael Lindner
// MIT license
//
#pragma once
#include "glm/vec3.hpp"


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

using Color = glm::vec3;
constexpr Color White{1.f, 1.f, 1.f};
constexpr Color Black{0.f, 0.f, 0.f};
constexpr Color NoColor = White;

using EntityId = int64_t;
