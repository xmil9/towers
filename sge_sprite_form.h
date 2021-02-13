//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "sge_coords.h"


namespace sge
{
///////////////////

// Describes a sprite's size and orientation in world space.
class SpriteForm
{
 public:
   SpriteForm() = default;
   SpriteForm(PixDim size);
   SpriteForm(PixDim size, Angle rot);
   SpriteForm(PixDim size, Angle rot, PixPos rotCenter);

   PixDim size() const { return m_size; }
   Angle rotation() const { return m_rot; }
   PixPos rotationCenter() const { return m_rotCenter; }

   void setSize(PixDim size);
   void setRotation(Angle rot) { m_rot = rot; }
   // Sets the center of rotation relative to the size of the sprite.
   void setRotationCenter(PixPos center) { m_rotCenter = center; }
   void scale(float factor) { m_size *= factor; }
   void rotate(Angle rot) { m_rot += rot; }

 private:
   PixDim m_size{0.f, 0.f};
   Angle m_rot{0.f};
   // Center of rotation relative to the size of the sprite, e.g. (size.x/2, size.y/2)
   // would be the center point of the sprite.
   PixPos m_rotCenter{0.f, 0.f};
};


inline SpriteForm::SpriteForm(PixDim size) : SpriteForm{size, Angle{0}}
{
}

inline SpriteForm::SpriteForm(PixDim size, Angle rot)
: SpriteForm{size, rot, PixPos(size.x * .5f, size.y * .5f)}
{
}

inline SpriteForm::SpriteForm(PixDim size, Angle rot, PixPos rotCenter)
: m_size{size}, m_rot{rot}, m_rotCenter{rotCenter}
{
}

} // namespace sge
