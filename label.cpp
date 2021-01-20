//
// Jan-2021, Michael Lindner
// MIT license
//
#include "label.h"
#include "renderer2.h"


///////////////////

void Label::setup(GetTextFn getTextFn, const PixPos& leftTop, const PixDim& dim)
{
   assert(getTextFn);

   m_getTextFn = getTextFn;
   m_leftTop = leftTop;
   m_dim = dim;
}


void Label::render(Renderer2& renderer, const PixPos& offset)
{
   renderer.renderText(m_getTextFn(), offset + m_leftTop, m_textScale, m_textColor);
}
