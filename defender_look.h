//
// Dec-2020, Michael Lindner
// MIT license
//
#pragma once
#include "animation.h"
#include "renderer2.h"
#include "sge_sprite.h"


///////////////////

class DefenderLook
{
 public:
   DefenderLook(const sge::Sprite& shape, Animation firing);
   DefenderLook(sge::Sprite&& shape, Animation firing);

   sge::PixDim size() const { return m_shape.size(); }

   DefenderLook& setSize(sge::PixDim size);
   DefenderLook& setRotation(sge::Angle_t rot);

   void render(Renderer2& renderer, sge::PixPos atCenter);
   void renderFiring(Renderer2& renderer, sge::PixPos atCenter, bool isPaused);

 private:
   sge::Sprite m_shape;
   Animation m_firing;
};


inline DefenderLook::DefenderLook(const sge::Sprite& shape, Animation firing)
: m_shape{shape}, m_firing{std::move(firing)}
{
}

inline DefenderLook::DefenderLook(sge::Sprite&& shape, Animation firing)
: m_shape{std::move(shape)}, m_firing{std::move(firing)}
{
}

inline DefenderLook& DefenderLook::setSize(sge::PixDim size)
{
   m_shape.setSize(size);
   m_firing.setSize(size);
   return *this;
}

inline DefenderLook& DefenderLook::setRotation(sge::Angle_t rot)
{
   m_shape.setRotation(rot);
   m_firing.setRotation(rot);
   return *this;
}

inline void DefenderLook::render(Renderer2& renderer, sge::PixPos atCenter)
{
   renderer.renderSpriteCentered(m_shape, atCenter);
}

inline void DefenderLook::renderFiring(Renderer2& renderer, sge::PixPos atCenter,
                                       bool isPaused)
{
   renderer.renderAnimationCentered(m_firing, atCenter, !isPaused);
}
