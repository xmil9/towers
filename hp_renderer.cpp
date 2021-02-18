//
// Jan-2021, Michael Lindner
// MIT license
//
#include "hp_renderer.h"
#include "spiel/renderer2.h"


void HpRenderer::render(sp::Renderer2& renderer, sp::PixPos atSpriteCenter, float ratio)
{
   // Adjust hp size to given ratio.
   const sp::PixDim fullDim = m_status.size();
   m_status.setSize(sp::PixDim{fullDim.x * ratio, fullDim.y});

   renderer.renderSprite(m_status, atSpriteCenter + m_offset);

   // Reset size.
   m_status.setSize(fullDim);
}
