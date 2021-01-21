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
   AttackerLook(const Sprite& shape, Animation explosion, HpRenderer* hpRenderer);
   AttackerLook(Sprite&& shape, Animation explosion, HpRenderer* hpRenderer);

   PixDim size() const { return m_shape.size(); }

   AttackerLook& setSize(PixDim size);
   AttackerLook& setRotation(Angle_t rot);

   void render(Renderer2& renderer, PixPos atCenter);
   void renderExploded(Renderer2& renderer, PixPos atCenter);

   bool hasExplosionFinished() const { return m_explosion.hasFinished(); }

 private:
   Sprite m_shape;
   Animation m_explosion;
   HpRenderer* m_hpRenderer = nullptr;
};


inline AttackerLook::AttackerLook(const Sprite& shape, Animation explosion,
                                  HpRenderer* hpRenderer)
: m_shape{shape}, m_explosion{std::move(explosion)}, m_hpRenderer{hpRenderer}
{
   assert(m_hpRenderer);
}

inline AttackerLook::AttackerLook(Sprite&& shape, Animation explosion,
                                  HpRenderer* hpRenderer)
: m_shape{std::move(shape)}, m_explosion{std::move(explosion)}, m_hpRenderer{hpRenderer}
{
   assert(m_hpRenderer);
}

inline AttackerLook& AttackerLook::setSize(PixDim size)
{
   m_shape.setSize(size);
   // Explosion stays the same size.
   return *this;
}

inline AttackerLook& AttackerLook::setRotation(Angle_t rot)
{
   m_shape.setRotation(rot);
   // Explosion is not rotated.
   return *this;
}

inline void AttackerLook::render(Renderer2& renderer, PixPos atCenter)
{
   const PixPos leftTop = atCenter - .5f * m_shape.size();
   renderer.renderSprite(m_shape, leftTop);
   m_hpRenderer->render(renderer, leftTop - PixVec{0.f, 20.f});
}

inline void AttackerLook::renderExploded(Renderer2& renderer, PixPos atCenter)
{
   // Also draw the attacker so that the explosion appears on top of it.
   render(renderer, atCenter);
   renderer.renderAnimation(m_explosion, atCenter - .5f * m_explosion.size());
}
