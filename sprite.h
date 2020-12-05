//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "sprite_form.h"
#include "sprite_look.h"
#include "sprite_renderer.h"

namespace gll
{
class Program;
}


///////////////////

class Sprite
{
 public:
   Sprite(SpriteRenderer& renderer, SpriteLook look);
   Sprite(SpriteRenderer& renderer, SpriteLook look, const SpriteForm& form);

   Sprite& setPosition(PixPos pos);
   Sprite& setSize(PixDim size);
   Sprite& setRotation(Angle_t rot);
   Sprite& setForm(const SpriteForm& form);

   void render(const gll::Program& shaders) const;

 private:
   SpriteRenderer& m_renderer;
   SpriteLook m_look;
   SpriteForm m_form;
};


inline Sprite::Sprite(SpriteRenderer& renderer, SpriteLook look)
: Sprite{renderer, look, SpriteForm{}}
{
}

inline Sprite::Sprite(SpriteRenderer& renderer, SpriteLook look, const SpriteForm& form)
: m_renderer{renderer}, m_look{look}, m_form{form}
{
}

inline Sprite& Sprite::setPosition(PixPos pos)
{
   m_form.setPosition(pos);
   return *this;
}

inline Sprite& Sprite::setSize(PixDim size)
{
   m_form.setSize(size);
   return *this;
}

inline Sprite& Sprite::setRotation(Angle_t rot)
{
   m_form.setRotation(rot);
   return *this;
}

inline Sprite& Sprite::setForm(const SpriteForm& form)
{
   m_form = form;
   return *this;
}

inline void Sprite::render(const gll::Program& shaders) const
{
   m_renderer.render(shaders, m_look, m_form);
}
