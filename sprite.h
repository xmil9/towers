//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "sprite_renderer.h"
#include <memory>

namespace gll
{
class Program;
}


class Sprite
{
 public:
   Sprite(std::shared_ptr<SpriteRenderer> renderer, std::string textureTag);

   void render(const gll::Program& shaders) const;

 private:
   std::shared_ptr<SpriteRenderer> m_renderer;
   std::string m_textureTag;
};


inline Sprite::Sprite(std::shared_ptr<SpriteRenderer> renderer, std::string textureTag)
: m_renderer{renderer}, m_textureTag{std::move(textureTag)}
{
}

inline void Sprite::render(const gll::Program& shaders) const
{
   m_renderer->render(shaders, m_textureTag, {10.0f, 10.0f});
}
