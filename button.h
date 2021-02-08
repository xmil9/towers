//
// Jan-2021, Michael Lindner
// MIT license
//
#pragma once
#include "coords.h"
#include "sge_sprite.h"
#include <functional>
#include <utility>

class Renderer2;


///////////////////

class Button
{
 public:
   using IsEnabledFn = std::function<bool()>;

 public:
   void setup(const sge::Sprite& background, sge::Sprite&& content,
              IsEnabledFn isEnabledFn, const sge::PixPos& leftTop,
              const sge::PixDim& dim);
   void setContent(sge::Sprite&& content);
   void render(Renderer2& renderer, const sge::PixPos& offset);
   bool isHit(const sge::PixPos& pos) const;
   bool isEnabled() const { return m_isEnabledFn(); }

 private:
   sge::Sprite m_background;
   sge::Sprite m_content;
   IsEnabledFn m_isEnabledFn;
   sge::PixPos m_leftTop{0.f, 0.f};
   sge::PixDim m_dim{0.f, 0.f};
};


inline void Button::setContent(sge::Sprite&& content)
{
   m_content = std::move(content);
}

inline bool Button::isHit(const sge::PixPos& pos) const
{
   return pos.x > m_leftTop.x && pos.x <= m_leftTop.x + m_dim.x && pos.y > m_leftTop.y &&
          pos.y <= m_leftTop.y + m_dim.y;
}
