//
// Jan-2021, Michael Lindner
// MIT license
//
#include "hp_renderer.h"
#include "renderer2.h"



void HpRenderer::render(Renderer2& renderer, sge::PixPos atSpriteCenter, float ratio)
{
   // Adjust hp size to given ratio.
   const sge::PixDim fullDim = m_status.size();
   m_status.setSize(sge::PixDim{fullDim.x * ratio, fullDim.y});

   renderer.renderSprite(m_status, atSpriteCenter + m_offset);

   // Reset size.
   m_status.setSize(fullDim);
}
