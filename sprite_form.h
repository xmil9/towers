//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "glm/vec2.hpp"


///////////////////

// Describes a sprite's configuration in world space.
struct SpriteForm
{
   glm::vec2 pos;
   glm::vec2 size;
   // Rotation angle in radians.
   float rot = 0.f;
};
