//
// Jan-2021, Michael Lindner
// MIT license
//
#include "button.h"
#include "renderer2.h"


///////////////////

static constexpr sge::Color DisabledTint{.8f, .8f, .8f};


///////////////////

void Button::setup(const sge::Sprite& background, sge::Sprite&& content,
                   IsEnabledFn isEnabledFn, const sge::PixPos& leftTop,
                   const sge::PixDim& dim)
{
   assert(isEnabledFn);

   m_background = background;
   m_content = std::move(content);
   m_isEnabledFn = isEnabledFn;
   m_leftTop = leftTop;
   m_dim = dim;
}


void Button::render(Renderer2& renderer, const sge::PixPos& offset)
{
   sge::Color tint = m_isEnabledFn() ? sge::NoColor : DisabledTint;

   renderer.renderSprite(m_background, offset + m_leftTop);
   renderer.renderSprite(m_content, offset + m_leftTop, tint);
}
