//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "basic_types.h"
#include "render_coords.h"


///////////////////

// Describes a sprite's configuration in world space.
struct SpriteForm
{
   // Position of left-top corner.
   RenderPos pos;
   RenderDim size;
   Angle_t rot{0.f};
};
