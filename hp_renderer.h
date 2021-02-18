//
// Jan-2021, Michael Lindner
// MIT license
//
#pragma once
#include "spiel/coords.h"
#include "spiel/sprite.h"

namespace sp
{
class Renderer2;
}


///////////////////

class HpRenderer
{
 public:
   HpRenderer(sp::Sprite&& status, sp::PixVec offset);

   void render(sp::Renderer2& renderer, sp::PixPos atSpriteCenter, float ratio);

 private:
   sp::Sprite m_status;
   sp::PixVec m_offset{0.f, 0.f};
};


inline HpRenderer::HpRenderer(sp::Sprite&& status, sp::PixVec offset)
: m_status{std::move(status)}, m_offset{offset}
{
}
