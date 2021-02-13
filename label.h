//
// Jan-2021, Michael Lindner
// MIT license
//
#pragma once
#include "sge_coords.h"
#include "sge_types.h"
#include <functional>
#include <string>

namespace sge
{
class Renderer2;
}


///////////////////

class Label
{
 public:
   using GetTextFn = std::function<std::string()>;

 public:
   Label(float textScale, const sge::Color& textColor);

   void setup(GetTextFn getTextFn, const sge::PixPos& leftTop, const sge::PixDim& dim);
   sge::PixPos position() const { return m_leftTop; }
   void setPosition(const sge::PixPos& leftTop) { m_leftTop = leftTop; }
   sge::PixDim dim() const { return m_dim; }
   void setDim(const sge::PixDim& dim) { m_dim = dim; }
   void render(sge::Renderer2& renderer, const sge::PixPos& offset);

 private:
   float m_textScale = 1.f;
   sge::Color m_textColor{sge::Black};
   GetTextFn m_getTextFn;
   sge::PixPos m_leftTop{0.f, 0.f};
   sge::PixDim m_dim{0.f, 0.f};
};


inline Label::Label(float textScale, const sge::Color& textColor)
: m_textScale{textScale}, m_textColor{textColor}
{
}
