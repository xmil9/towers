//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "render_coords.h"


///////////////////

// Describes a sprite's configuration in world space.
struct SpriteForm
{
   // Position of left-top corner.
   RenderPos pos;
   RenderDim size;
   // Rotation angle in radians.
   float rot = 0.f;
};
