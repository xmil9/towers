//
// Jan-2021, Michael Lindner
// MIT license
//
#pragma once
#include "coords.h"
#include "sge_sprite.h"

class Renderer2;


///////////////////

class HpRenderer
{
public:
   HpRenderer(sge::Sprite&& status, sge::PixVec offset);

   void render(Renderer2& renderer, sge::PixPos atSpriteCenter, float ratio);

private:
   sge::Sprite m_status;
   sge::PixVec m_offset{0.f, 0.f};
};


inline HpRenderer::HpRenderer(sge::Sprite&& status, sge::PixVec offset)
   : m_status{std::move(status)}, m_offset{offset}
{
}
