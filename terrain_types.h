//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "glm/vec2.hpp"


// Position in terrain coordinates. Coordinates are in range [0.0, 1.0].
using Pos = glm::vec2;

struct Rect
{
   Pos lt;
   Pos rb;

   bool contains(Pos at) const;
};

inline bool Rect::contains(Pos at) const
{
   return lt.x <= at.x && at.x < rb.x && lt.y <= at.y && at.y < rb.y;
}


// Indices of field.
using FieldPos = glm::ivec2;
