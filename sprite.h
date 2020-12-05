//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "sprite_form.h"
#include "sprite_look.h"
#include "sprite_renderer.h"
#include <memory>

namespace gll
{
class Program;
}


///////////////////

class Sprite
{
 public:
   Sprite(SpriteRenderer& renderer, std::shared_ptr<SpriteLook> look);
   Sprite(SpriteRenderer& renderer, std::shared_ptr<SpriteLook> look,
          const SpriteForm& form);

   Sprite& setPosition(RenderPos pos);
   Sprite& setSize(RenderDim size);
   Sprite& setRotation(float rot);
   Sprite& setForm(const SpriteForm& form);

   void render(const gll::Program& shaders) const;

 private:
   SpriteRenderer& m_renderer;
   std::shared_ptr<SpriteLook> m_look;
   SpriteForm m_form;
};


inline Sprite::Sprite(SpriteRenderer& renderer,
                      std::shared_ptr<SpriteLook> look)
   : Sprite{renderer, look, SpriteForm{}}
{
}

inline Sprite::Sprite(SpriteRenderer& renderer,
                      std::shared_ptr<SpriteLook> look, const SpriteForm& form)
: m_renderer{renderer}, m_look{look}, m_form{form}
{
}

inline Sprite& Sprite::setPosition(RenderPos pos)
{
   m_form.m_pos = pos;
   return *this;
}

inline Sprite& Sprite::setSize(RenderDim size)
{
   m_form.m_size = size;
   return *this;
}

inline Sprite& Sprite::setRotation(float rot)
{
   m_form.m_rot = rot;
   return *this;
}

inline Sprite& Sprite::setForm(const SpriteForm& form)
{
   m_form = form;
   return *this;
}

inline void Sprite::render(const gll::Program& shaders) const
{
   m_renderer.render(shaders, *m_look, m_form);
}
