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
   SpriteForm(sge::PixDim size);
   SpriteForm(sge::PixDim size, Angle_t rot);
   SpriteForm(sge::PixDim size, Angle_t rot, sge::PixPos rotCenter);

   sge::PixDim size() const { return m_size; }
   Angle_t rotation() const { return m_rot; }
   sge::PixPos rotationCenter() const { return m_rotCenter; }

   void setSize(sge::PixDim size);
   void setRotation(Angle_t rot) { m_rot = rot; }
   // Sets the center of rotation relative to the size of the sprite.
   void setRotationCenter(sge::PixPos center) { m_rotCenter = center; }
   void scale(float factor) { m_size *= factor; }
   void rotate(Angle_t rot) { m_rot += rot; }

 private:
   sge::PixDim m_size{0.f, 0.f};
   Angle_t m_rot{0.f};
   // Center of rotation relative to the size of the sprite, e.g. (size.x/2, size.y/2)
   // would be the center point of the sprite.
   sge::PixPos m_rotCenter{0.f, 0.f};
};


inline SpriteForm::SpriteForm(sge::PixDim size) : SpriteForm{size, Angle_t{0}}
{
}

inline SpriteForm::SpriteForm(sge::PixDim size, Angle_t rot)
: SpriteForm{size, rot, sge::PixPos(size.x * .5f, size.y * .5f)}
{
}

inline SpriteForm::SpriteForm(sge::PixDim size, Angle_t rot, sge::PixPos rotCenter)
: m_size{size}, m_rot{rot}, m_rotCenter{rotCenter}
{
}
