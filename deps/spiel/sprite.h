//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "sprite_form.h"
#include "sprite_look.h"


namespace sp
{
///////////////////

class Sprite
{
 public:
   Sprite() = default;
   explicit Sprite(SpriteLook look);
   Sprite(SpriteLook look, const SpriteForm& form);

   PixDim size() const { return m_form.size(); }
   Angle rotation() const { return m_form.rotation(); }
   PixPos rotationCenter() const { return m_form.rotationCenter(); }
   bool hasTexture() const { return m_look.hasTexture(); }
   std::string texture() const { return m_look.texture(); }
   Color color() const { return m_look.color(); }

   Sprite& setSize(PixDim size);
   Sprite& setRotation(Angle rot);
   Sprite& setForm(const SpriteForm& form);
   Sprite& scale(float factor);
   Sprite& rotate(Angle rot);

 private:
   SpriteLook m_look;
   SpriteForm m_form;
};


inline Sprite::Sprite(SpriteLook look) : Sprite{look, SpriteForm{}}
{
}

inline Sprite::Sprite(SpriteLook look, const SpriteForm& form)
: m_look{std::move(look)}, m_form{form}
{
}

inline Sprite& Sprite::setSize(PixDim size)
{
   m_form.setSize(size);
   return *this;
}

inline Sprite& Sprite::setRotation(Angle rot)
{
   m_form.setRotation(rot);
   return *this;
}

inline Sprite& Sprite::setForm(const SpriteForm& form)
{
   m_form = form;
   return *this;
}

inline Sprite& Sprite::scale(float factor)
{
   m_form.scale(factor);
   return *this;
}

inline Sprite& Sprite::rotate(Angle rot)
{
   m_form.rotate(rot);
   return *this;
}

} // namespace sp
