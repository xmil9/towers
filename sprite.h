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
   Sprite() = default;
   Sprite(SpriteRenderer* renderer, SpriteLook look);
   Sprite(SpriteRenderer* renderer, SpriteLook look, const SpriteForm& form);

   PixDim size() const { return m_form.size(); }

   Sprite& setSize(PixDim size);
   Sprite& setRotation(Angle_t rot);
   Sprite& setForm(const SpriteForm& form);
   Sprite& scale(float factor);
   Sprite& rotate(Angle_t rot);

   void render(const gll::Program& shaders, PixPos at) const;

 private:
   SpriteRenderer* m_renderer = nullptr;
   SpriteLook m_look;
   SpriteForm m_form;
};


inline Sprite::Sprite(SpriteRenderer* renderer, SpriteLook look)
: Sprite{renderer, look, SpriteForm{}}
{
}

inline Sprite::Sprite(SpriteRenderer* renderer, SpriteLook look, const SpriteForm& form)
: m_renderer{renderer}, m_look{look}, m_form{form}
{
   assert(m_renderer);
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

inline Sprite& Sprite::scale(float factor)
{
   m_form.scale(factor);
   return *this;
}

inline Sprite& Sprite::rotate(Angle_t rot)
{
   m_form.rotate(rot);
   return *this;
}

inline void Sprite::render(const gll::Program& shaders, PixPos at) const
{
   m_renderer->render(shaders, m_look, m_form, at);
}
