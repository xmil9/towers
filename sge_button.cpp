//
// Jan-2021, Michael Lindner
// MIT license
//
#include "sge_button.h"
#include "sge_renderer2.h"


///////////////////

static constexpr sge::Color DisabledTint{.8f, .8f, .8f};


namespace sge
{
///////////////////

void Button::setup(const Sprite& background, Sprite&& content,
                   IsEnabledFn isEnabledFn, const PixPos& leftTop,
                   const PixDim& dim)
{
   assert(isEnabledFn);

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

} // namespace sge
