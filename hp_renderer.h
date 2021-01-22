//
// Jan-2021, Michael Lindner
// MIT license
//
#pragma once
#include "coords.h"
#include "sprite.h"

class Renderer2;


///////////////////

class HpRenderer
{
public:
   HpRenderer(Sprite&& status, PixVec offset);

   void render(Renderer2& renderer, PixPos atSpriteCenter, float ratio);

private:
   Sprite m_status;
   PixVec m_offset{0.f, 0.f};
};


inline HpRenderer::HpRenderer(Sprite&& status, PixVec offset)
   : m_status{std::move(status)}, m_offset{offset}
{
}
