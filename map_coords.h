//
// Jan-2021, Michael Lindner
// MIT license
//
#pragma once
#include "rect.h"
#include "glm/vec2.hpp"


///////////////////

// Map coordinates expressed in floating point field indices.
// Range: ([0.f, num columns in map] x [0.f, num rows in map]).
// These represent exact map locations, e.g. (1.5, 2.5) is the center point of
// the field in column 1 (zero-based) and row 2 (zero-based).

using MapCoord = float;
using MapPos = glm::vec2;
using MapVec = glm::vec2;
using MapDim = glm::vec2;
using MapRect = Rect<MapPos>;

constexpr MapVec Up{0.f, -1.f};
constexpr MapVec Down{0.f, 1.f};
constexpr MapVec Left{-1.f, 0.f};
constexpr MapVec Right{1.f, 0.f};

