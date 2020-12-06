//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "coords.h"


///////////////////

// Describes a sprite's location in world space.
class SpriteForm
{
 public:
   SpriteForm() = default;
   SpriteForm(PixPos pos, PixDim size, Angle_t rot);

   PixPos position() const { return m_pos; }
   PixDim size() const { return m_size; }
   Angle_t rotation() const { return m_rot; }

   void setPosition(PixPos pos) { m_pos = pos; }
   void setSize(PixDim size) { m_size = size; }
   void setRotation(Angle_t rot) { m_rot = rot; }

 private:
   // Position of left-top corner.
   PixPos m_pos;
   PixDim m_size;
   Angle_t m_rot{0.f};
};


inline SpriteForm::SpriteForm(PixPos pos, PixDim size, Angle_t rot)
: m_pos{pos}, m_size{size}, m_rot{rot}
{
}
