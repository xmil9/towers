//
// Dec-2020, Michael Lindner
// MIT license
//
#pragma once
#include "glm/vec2.hpp"
#include "essentutils/fputil.h"


///////////////////

// Position in render coordinates. Coordinates are in range [0, map width in pixels]x[0,
// map height in pixels].
using RenderPos = glm::vec2;

// Vector in render coordinates.
using RenderVec = glm::vec2;

// Dimensions in render coordinates.
using RenderDim = glm::vec2;
