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

// Normalized coordinates are in range [0.0, 1.0].

using NormCoord = float;
using NormPos = glm::vec2;
using NormVec = glm::vec2;
using NormDim = glm::vec2;
using NormRect = sge::Rect<NormPos>;

} // namespace sge
