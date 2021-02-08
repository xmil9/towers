//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "sge_sprite_form.h"
#include "sge_sprite_look.h"


///////////////////

class Sprite
{
 public:
   Sprite() = default;
   explicit Sprite(sge::SpriteLook look);
   Sprite(sge::SpriteLook look, const sge::SpriteForm& form);

   sge::PixDim size() const { return m_form.size(); }
   sge::Angle_t rotation() const { return m_form.rotation(); }
   sge::PixPos rotationCenter() const { return m_form.rotationCenter(); }
   bool hasTexture() const { return m_look.hasTexture(); }
   std::string texture() const { return m_look.texture(); }
   sge::Color color() const { return m_look.color(); }

   Sprite& setSize(sge::PixDim size);
   Sprite& setRotation(sge::Angle_t rot);
   Sprite& setForm(const sge::SpriteForm& form);
   Sprite& scale(float factor);
   Sprite& rotate(sge::Angle_t rot);

 private:
   sge::SpriteLook m_look;
   sge::SpriteForm m_form;
};


inline Sprite::Sprite(sge::SpriteLook look) : Sprite{look, sge::SpriteForm{}}
{
}

inline Sprite::Sprite(sge::SpriteLook look, const sge::SpriteForm& form)
: m_look{std::move(look)}, m_form{form}
{
}

inline Sprite& Sprite::setSize(sge::PixDim size)
{
   m_form.setSize(size);
   return *this;
}

inline Sprite& Sprite::setRotation(sge::Angle_t rot)
{
   m_form.setRotation(rot);
   return *this;
}

inline Sprite& Sprite::setForm(const sge::SpriteForm& form)
{
   m_form = form;
   return *this;
}

inline Sprite& Sprite::scale(float factor)
{
   m_form.scale(factor);
   return *this;
}

inline Sprite& Sprite::rotate(sge::Angle_t rot)
{
   m_form.rotate(rot);
   return *this;
}
