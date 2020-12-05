//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "basic_types.h"
#include "render_coords.h"


///////////////////

// Describes a sprite's configuration in world space.
class SpriteForm
{
public:
   SpriteForm() = default;
   //SpriteForm(RenderPos pos, RenderDim size, Angle_t rot);
   //SpriteForm(RenderPos pos, float maxSize, Angle_t rot);

//private:
   // Position of left-top corner.
   RenderPos m_pos;
   RenderDim m_size;
   Angle_t m_rot{0.f};
};
