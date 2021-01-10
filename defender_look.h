//
// Dec-2020, Michael Lindner
// MIT license
//
#pragma once
#include "animation.h"
#include "renderer2.h"
#include "sprite.h"


///////////////////

class DefenderLook
{
 public:
   DefenderLook(const Sprite& shape, Animation firing);
   DefenderLook(Sprite&& shape, Animation firing);

   PixDim size() const { return m_shape.size(); }

   DefenderLook& setSize(PixDim size);
   DefenderLook& setRotation(Angle_t rot);

   void render(const Renderer2& renderer, PixPos atCenter);
   void renderFiring(const Renderer2& renderer, PixPos atCenter);

 private:
   Sprite m_shape;
   Animation m_firing;
};


inline DefenderLook::DefenderLook(const Sprite& shape, Animation firing)
: m_shape{shape}, m_firing{std::move(firing)}
{
}

inline DefenderLook::DefenderLook(Sprite&& shape, Animation firing)
: m_shape{std::move(shape)}, m_firing{std::move(firing)}
{
}

inline DefenderLook& DefenderLook::setSize(PixDim size)
{
   m_shape.setSize(size);
   m_firing.setSize(size);
   return *this;
}

inline DefenderLook& DefenderLook::setRotation(Angle_t rot)
{
   m_shape.setRotation(rot);
   m_firing.setRotation(rot);
   return *this;
}

inline void DefenderLook::render(const Renderer2& renderer, PixPos atCenter)
{
   renderer.renderSprite(m_shape, atCenter - .5f * m_shape.size());
}

inline void DefenderLook::renderFiring(const Renderer2& renderer, PixPos atCenter)
{
   renderer.renderAnimation(m_firing, atCenter - .5f * m_firing.size());
}
