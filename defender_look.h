//
// Dec-2020, Michael Lindner
// MIT license
//
#pragma once
#include "spiel/animation.h"
#include "spiel/renderer2.h"
#include "spiel/sprite.h"


///////////////////

class DefenderLook
{
 public:
   DefenderLook(const sp::Sprite& shape, sp::Animation firing);
   DefenderLook(sp::Sprite&& shape, sp::Animation firing);

   sp::PixDim size() const { return m_shape.size(); }

   DefenderLook& setSize(sp::PixDim size);
   DefenderLook& setRotation(sp::Angle rot);

   void render(sp::Renderer2& renderer, sp::PixPos atCenter);
   void renderFiring(sp::Renderer2& renderer, sp::PixPos atCenter, bool isPaused);

 private:
   sp::Sprite m_shape;
   sp::Animation m_firing;
};


inline DefenderLook::DefenderLook(const sp::Sprite& shape, sp::Animation firing)
: m_shape{shape}, m_firing{std::move(firing)}
{
}

inline DefenderLook::DefenderLook(sp::Sprite&& shape, sp::Animation firing)
: m_shape{std::move(shape)}, m_firing{std::move(firing)}
{
}

inline DefenderLook& DefenderLook::setSize(sp::PixDim size)
{
   m_shape.setSize(size);
   m_firing.setSize(size);
   return *this;
}

inline DefenderLook& DefenderLook::setRotation(sp::Angle rot)
{
   m_shape.setRotation(rot);
   m_firing.setRotation(rot);
   return *this;
}

inline void DefenderLook::render(sp::Renderer2& renderer, sp::PixPos atCenter)
{
   renderer.renderSpriteCentered(m_shape, atCenter);
}

inline void DefenderLook::renderFiring(sp::Renderer2& renderer, sp::PixPos atCenter,
                                       bool isPaused)
{
   renderer.renderAnimationCentered(m_firing, atCenter, !isPaused);
}
