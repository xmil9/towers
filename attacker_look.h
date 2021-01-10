//
// Dec-2020, Michael Lindner
// MIT license
//
#pragma once
#include "animation.h"
#include "renderer2.h"
#include "sprite.h"


///////////////////

class AttackerLook
{
 public:
   AttackerLook(const Sprite& shape, Animation explosion);
   AttackerLook(Sprite&& shape, Animation explosion);

   PixDim size() const { return m_shape.size(); }

   AttackerLook& setSize(PixDim size);
   AttackerLook& setRotation(Angle_t rot);

   void render(const Renderer2& renderer, PixPos atCenter);
   void renderExploded(const Renderer2& renderer, PixPos atCenter);

   bool hasExplosionFinished() const { return m_explosion.hasFinished(); }

 private:
   Sprite m_shape;
   Animation m_explosion;
};


inline AttackerLook::AttackerLook(const Sprite& shape, Animation explosion)
: m_shape{shape}, m_explosion{std::move(explosion)}
{
}

inline AttackerLook::AttackerLook(Sprite&& shape, Animation explosion)
: m_shape{std::move(shape)}, m_explosion{std::move(explosion)}
{
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

inline void AttackerLook::render(const Renderer2& renderer, PixPos atCenter)
{
   renderer.renderSprite(m_shape, atCenter - .5f * m_shape.size());
}

inline void AttackerLook::renderExploded(const Renderer2& renderer, PixPos atCenter)
{
   // Also draw the attacker so that the explosion appears on top of it.
   render(renderer, atCenter);
   renderer.renderAnimation(m_explosion, atCenter - .5f * m_explosion.size());
}
