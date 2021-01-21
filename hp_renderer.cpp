//
// Jan-2021, Michael Lindner
// MIT license
//
#include "hp_renderer.h"
#include "renderer2.h"



void HpRenderer::setup(Sprite&& hpMeter, Sprite&& hpStatus)
{
   m_hpMeter = std::move(hpMeter);
   m_hpStatus = std::move(hpStatus);
}


void HpRenderer::render(Renderer2& renderer, PixPos at)
{
   renderer.renderSprite(m_hpMeter, at);
   renderer.renderSprite(m_hpStatus, at);
}
