//
// Dec-2020, Michael Lindner
// MIT license
//
#pragma once
#include "animation.h"
#include "hp_renderer.h"
#include "renderer2.h"
#include "sprite.h"
#include "texture_tags.h"


///////////////////

class AttackerLook
{
 public:
   AttackerLook(Sprite&& shape, Sprite&& shapeHit, Animation explosion,
                HpRenderer* hpRenderer);

   PixDim size() const { return m_shape.size(); }

   AttackerLook& setSize(PixDim size);
   AttackerLook& setRotation(Angle_t rot);

   void render(Renderer2& renderer, PixPos atCenter, bool isAlive, float hpRatio,
               bool isHit);
   void renderExploded(Renderer2& renderer, PixPos atCenter);

   bool hasExplosionFinished() const { return m_explosion.hasFinished(); }

 private:
   Sprite m_shape;
   Sprite m_shapeHit;
   Animation m_explosion;
   HpRenderer* m_hpRenderer = nullptr;
};


inline AttackerLook::AttackerLook(Sprite&& shape, Sprite&& shapeHit, Animation explosion,
                                  HpRenderer* hpRenderer)
: m_shape{std::move(shape)}, m_shapeHit{std::move(shapeHit)},
  m_explosion{std::move(explosion)}, m_hpRenderer{hpRenderer}
{
   assert(m_hpRenderer);
}

inline AttackerLook& AttackerLook::setSize(PixDim size)
{
   m_shape.setSize(size);
   m_shapeHit.setSize(size);
   // Explosion stays the same size.
   // Hp bar stays the same size.
   return *this;
}

inline AttackerLook& AttackerLook::setRotation(Angle_t rot)
{
   m_shape.setRotation(rot);
   m_shapeHit.setRotation(rot);
   // Explosion is not rotated.
   // Hp bar is not rotated.
   return *this;
}

inline void AttackerLook::render(Renderer2& renderer, PixPos atCenter, bool isAlive,
                                 float hpRatio, bool isHit)
{
   if (isHit)
      renderer.renderSpriteCentered(m_shapeHit, atCenter);
   else
      renderer.renderSpriteCentered(m_shape, atCenter);

   if (isAlive)
      m_hpRenderer->render(renderer, atCenter, hpRatio);
}

inline void AttackerLook::renderExploded(Renderer2& renderer, PixPos atCenter)
{
   // Also draw the attacker so that the explosion appears on top of it.
   render(renderer, atCenter, false, 0.f, true);
   renderer.renderAnimation(m_explosion, atCenter - .5f * m_explosion.size());
}
