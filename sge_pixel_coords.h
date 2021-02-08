//
// Dec-2020, Michael Lindner
// MIT license
//
#pragma once
#include "sge_rect.h"
#include "glm/vec2.hpp"


namespace sge
{
///////////////////

// Pixel coordinates represented by floating point values.
using PixCoord = float;
using PixPos = glm::vec2;
using PixVec = glm::vec2;
using PixDim = glm::vec2;
using PixRect = sge::Rect<PixPos>;

// Pixel coordinates represented by integer values.
using PixCoordi = int;
using PixPosi = glm::ivec2;
using PixVeci = glm::ivec2;
using PixDimi = glm::ivec2;
using PixRecti = sge::Rect<PixPosi>;

} // namespace sge
