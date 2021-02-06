//
// Dec-2020, Michael Lindner
// MIT license
//
#pragma once
#include "sge_rect.h"
#include "glm/vec2.hpp"


///////////////////

// Integer coordinate.
// E.g. for representing the rows and columns on a board.

using IntCoord = int;
using IntPos = glm::ivec2;
using IntVec = glm::ivec2;
using IntDim = glm::ivec2;
using IntRect = sge::Rect<IntPos>;
