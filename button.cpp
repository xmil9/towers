//
// Jan-2021, Michael Lindner
// MIT license
//
#include "button.h"


///////////////////

static constexpr Color DisabledTint{.8f, .8f, .8f};


///////////////////

void Button::setup(const Sprite& background, Sprite&& content, IsEnabledFn isEnabledFn,
                   const PixPos& leftTop, const PixDim& dim)
{
   m_background = background;
   m_content = std::move(content);
   m_isEnabledFn = isEnabledFn;
   m_leftTop = leftTop;
   m_dim = dim;
}


void Button::render(Renderer2& renderer, const PixPos& offset)
{
   Color tint = m_isEnabledFn() ? NoColor : DisabledTint;

   renderer.renderSprite(m_background, offset + m_leftTop);
   renderer.renderSprite(m_content, offset + m_leftTop, tint);
}
