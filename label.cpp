//
// Jan-2021, Michael Lindner
// MIT license
//
#include "label.h"
#include "sge_renderer2.h"


///////////////////

void Label::setup(GetTextFn getTextFn, const sge::PixPos& leftTop, const sge::PixDim& dim)
{
   assert(getTextFn);

   m_getTextFn = getTextFn;
   m_leftTop = leftTop;
   m_dim = dim;
}


void Label::render(sge::Renderer2& renderer, const sge::PixPos& offset)
{
   renderer.renderText(m_getTextFn(), offset + m_leftTop, m_textScale, m_textColor);
}
