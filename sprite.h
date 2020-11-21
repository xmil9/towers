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
   Sprite(std::shared_ptr<SpriteRenderer> renderer, std::shared_ptr<SpriteLook> look);
   Sprite(std::shared_ptr<SpriteRenderer> renderer, std::shared_ptr<SpriteLook> look,
          const SpriteForm& form);

   Sprite& setPosition(glm::vec2 pos);
   Sprite& setSize(glm::vec2 size);
   Sprite& setRotation(float rot);
   Sprite& setForm(const SpriteForm& form);

   void render(const gll::Program& shaders) const;

 private:
   std::shared_ptr<SpriteRenderer> m_renderer;
   std::shared_ptr<SpriteLook> m_look;
   SpriteForm m_form;
};


inline Sprite::Sprite(std::shared_ptr<SpriteRenderer> renderer,
                      std::shared_ptr<SpriteLook> look)
   : Sprite{renderer, look, SpriteForm{}}
{
}

inline Sprite::Sprite(std::shared_ptr<SpriteRenderer> renderer,
                      std::shared_ptr<SpriteLook> look, const SpriteForm& form)
: m_renderer{renderer}, m_look{look}, m_form{form}
{
}

inline Sprite& Sprite::setPosition(glm::vec2 pos)
{
   m_form.pos = pos;
   return *this;
}

inline Sprite& Sprite::setSize(glm::vec2 size)
{
   m_form.size = size;
   return *this;
}

inline Sprite& Sprite::setRotation(float rot)
{
   m_form.rot = rot;
   return *this;
}

inline Sprite& Sprite::setForm(const SpriteForm& form)
{
   m_form = form;
   return *this;
}

inline void Sprite::render(const gll::Program& shaders) const
{
   m_renderer->render(shaders, *m_look, m_form);
}
