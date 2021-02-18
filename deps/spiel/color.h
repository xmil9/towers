//
// Feb-2021, Michael Lindner
// MIT license
//
#pragma once
#include "glm/vec3.hpp"


namespace sp
{
///////////////////

using Color = glm::vec3;
constexpr Color White{1.f, 1.f, 1.f};
constexpr Color Black{0.f, 0.f, 0.f};
constexpr Color NoColor = White;

} // namespace sp
