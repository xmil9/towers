//
// Jan-2021, Michael Lindner
// MIT license
//
#pragma once
#include "coords.h"
#include "sprite.h"
#include <functional>

class Renderer2;


///////////////////

class Button
{
 public:
   using IsEnabledFn = std::function<bool()>;

 public:
   void setup(const Sprite& background, Sprite&& content, IsEnabledFn isEnabledFn,
              const PixPos& leftTop, const PixDim& dim);
   void render(Renderer2& renderer, const PixPos& offset);
   bool isHit(const PixPos& pos) const;
   bool isEnabled() const { return m_isEnabledFn(); }

 private:
   Sprite m_background;
   Sprite m_content;
   IsEnabledFn m_isEnabledFn;
   PixPos m_leftTop{0.f, 0.f};
   PixDim m_dim{0.f, 0.f};
};


inline bool Button::isHit(const PixPos& pos) const
{
   return pos.x > m_leftTop.x && pos.x <= m_leftTop.x + m_dim.x && pos.y > m_leftTop.y &&
          pos.y <= m_leftTop.y + m_dim.y;
}
