//
// Dec-2020, Michael Lindner
// MIT license
//
#pragma once
#include "map_coords.h"
#include "sge_coords.h"
#include "essentutils/fputil.h"


///////////////////

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
