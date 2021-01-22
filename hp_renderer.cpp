//
// Jan-2021, Michael Lindner
// MIT license
//
#include "hp_renderer.h"
#include "renderer2.h"



void HpRenderer::setup(Sprite&& hpMeter, Sprite&& hpStatus, PixVec offset)
{
   m_hpMeter = std::move(hpMeter);
   m_hpStatus = std::move(hpStatus);
   m_offset = offset;
}


void HpRenderer::render(Renderer2& renderer, PixPos atSpriteCenter, float ratio)
{
   //renderer.renderSprite(m_hpMeter, at);
   const PixDim fullDim = m_hpStatus.size();
   PixDim dim = fullDim;
   dim.x = fullDim.x * ratio;
   m_hpStatus.setSize(dim);

   renderer.renderSprite(m_hpStatus, atSpriteCenter + m_offset);

   m_hpStatus.setSize(fullDim);
}
