//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "coords.h"


///////////////////

// Describes a sprite's size and orientation in world space.
class SpriteForm
{
 public:
   SpriteForm() = default;
   SpriteForm(PixDim size, Angle_t rot);

   PixDim size() const { return m_size; }
   Angle_t rotation() const { return m_rot; }

   void setSize(PixDim size) { m_size = size; }
   void setRotation(Angle_t rot) { m_rot = rot; }

 private:
   PixDim m_size;
   Angle_t m_rot{0.f};
};


inline SpriteForm::SpriteForm(PixDim size, Angle_t rot)
: m_size{size}, m_rot{rot}
{
}
