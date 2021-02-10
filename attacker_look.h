//
// Dec-2020, Michael Lindner
// MIT license
//
#pragma once
#include "hp_renderer.h"
#include "texture_tags.h"
#include "sge_animation.h"
#include "sge_renderer2.h"
#include "sge_sprite.h"


///////////////////

class AttackerLook
{
 public:
   AttackerLook(sge::Sprite&& shape, sge::Sprite&& shapeHit, sge::Animation explosion,
                HpRenderer* hpRenderer);

   sge::PixDim size() const { return m_shape.size(); }

   AttackerLook& setSize(sge::PixDim size);
   AttackerLook& setRotation(sge::Angle_t rot);

   void render(sge::Renderer2& renderer, sge::PixPos atCenter, bool isAlive,
               float hpRatio, bool isHit);
   void renderExploded(sge::Renderer2& renderer, sge::PixPos atCenter, bool isPaused);

   bool hasExplosionFinished() const { return m_explosion.hasFinished(); }

 private:
   sge::Sprite m_shape;
   sge::Sprite m_shapeHit;
   sge::Animation m_explosion;
   HpRenderer* m_hpRenderer = nullptr;
};


inline AttackerLook::AttackerLook(sge::Sprite&& shape, sge::Sprite&& shapeHit,
                                  sge::Animation explosion, HpRenderer* hpRenderer)
: m_shape{std::move(shape)}, m_shapeHit{std::move(shapeHit)},
  m_explosion{std::move(explosion)}, m_hpRenderer{hpRenderer}
{
   assert(m_hpRenderer);
}

inline AttackerLook& AttackerLook::setSize(sge::PixDim size)
{
   m_shape.setSize(size);
   m_shapeHit.setSize(size);
   // Explosion stays the same size.
   // Hp bar stays the same size.
   return *this;
}

inline AttackerLook& AttackerLook::setRotation(sge::Angle_t rot)
{
   m_shape.setRotation(rot);
   m_shapeHit.setRotation(rot);
   // Explosion is not rotated.
   // Hp bar is not rotated.
   return *this;
}

inline void AttackerLook::render(sge::Renderer2& renderer, sge::PixPos atCenter,
                                 bool isAlive, float hpRatio, bool isHit)
{
   if (isHit)
      renderer.renderSpriteCentered(m_shapeHit, atCenter);
   else
      renderer.renderSpriteCentered(m_shape, atCenter);

   if (isAlive)
      m_hpRenderer->render(renderer, atCenter, hpRatio);
}

inline void AttackerLook::renderExploded(sge::Renderer2& renderer, sge::PixPos atCenter,
                                         bool isPaused)
{
   // Also draw the attacker so that the explosion appears on top of it.
   render(renderer, atCenter, false, 0.f, true);
   renderer.renderAnimationCentered(m_explosion, atCenter, !isPaused);
}
