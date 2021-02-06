//
// Dec-2020, Michael Lindner
// MIT license
//
#pragma once
#include "int_coords.h"
#include "map_coords.h"
#include "norm_coords.h"
#include "pixel_coords.h"
#include "essentutils/angle.h"
#include "essentutils/fputil.h"


///////////////////

using Angle_t = esl::Angle<float>;


// Equality for any values represented by GLM floating point vectors.
inline bool isEqual(glm::vec2 a, glm::vec2 b)
{
   return esl::equal(a.x, b.x) && esl::equal(a.y, b.y);
}

// Equality for any values represented by GLM integer vectors.
// Not really necessary because integer values have no precision issues but nice to have
// for generic code.
inline bool isEqual(glm::ivec2 a, glm::ivec2 b)
{
   return esl::equal(a.x, b.x) && esl::equal(a.y, b.y);
}
