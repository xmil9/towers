//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "sprite_look.h"
#include "sprite_renderer.h"
#include <memory>

namespace gll
{
class Program;
}


class Sprite
{
 public:
   Sprite(std::shared_ptr<SpriteRenderer> renderer, std::shared_ptr<SpriteLook> look);

   void render(const gll::Program& shaders) const;

 private:
   std::shared_ptr<SpriteRenderer> m_renderer;
   std::shared_ptr<SpriteLook> m_look;
};


inline Sprite::Sprite(std::shared_ptr<SpriteRenderer> renderer,
                      std::shared_ptr<SpriteLook> look)
: m_renderer{renderer}, m_look{look}
{
}

inline void Sprite::render(const gll::Program& shaders) const
{
   m_renderer->render(shaders, *m_look, {5.f, 25.f}, {100.f, 100.f}, .5f);
}
