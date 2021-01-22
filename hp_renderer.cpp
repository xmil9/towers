//
// Jan-2021, Michael Lindner
// MIT license
//
#include "hp_renderer.h"
#include "renderer2.h"



void HpRenderer::setup(Sprite&& status, PixVec offset)
{
   m_status = std::move(status);
   m_offset = offset;
}


void HpRenderer::render(Renderer2& renderer, PixPos atSpriteCenter, float ratio)
{
   const PixDim fullDim = m_status.size();
   m_status.setSize(PixDim{fullDim.x * ratio, fullDim.y});

   renderer.renderSprite(m_status, atSpriteCenter + m_offset);

   // Reset size.
   m_status.setSize(fullDim);
}
