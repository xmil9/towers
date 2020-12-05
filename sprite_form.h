//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "basic_types.h"
#include "render_coords.h"


///////////////////

// Describes a sprite's location in world space.
class SpriteForm
{
public:
   SpriteForm() = default;
   SpriteForm(RenderPos pos, RenderDim size, Angle_t rot);

   RenderPos position() const { return m_pos; }
   RenderDim size() const { return m_size; }
   Angle_t rotation() const { return m_rot; }

   void setPosition(RenderPos pos) { m_pos = pos; }
   void setSize(RenderDim size) { m_size = size; }
   void setRotation(Angle_t rot) { m_rot = rot; }

private:
   // Position of left-top corner.
   RenderPos m_pos;
   RenderDim m_size;
   Angle_t m_rot{0.f};
};


inline SpriteForm::SpriteForm(RenderPos pos, RenderDim size, Angle_t rot)
   : m_pos{pos}, m_size{size}, m_rot{rot}
{
}
