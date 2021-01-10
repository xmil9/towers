//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "camera_2d.h"
#include "frustum2.h"
#include "sprite_renderer.h"

class Animation;
class Resources;
class Sprite;


///////////////////

class Renderer2
{
public:
   bool setup(Resources* resources, int viewWidth, int viewHeight);
   void setFrustumSize(int width, int height) { m_frustum.setSize(width, height); }

   void beginRendering(bool clear) const;
   void renderSprite(const Sprite& sprite, PixPos leftTop) const;
   void renderAnimation(Animation& anim, PixPos leftTop) const;

private:
   bool setupSetting(int viewWidth, int viewHeight);

private:
   Resources* m_resources = nullptr;
   Camera2d m_cam;
   Frustum2 m_frustum;
   std::unique_ptr<SpriteRenderer> m_spriteRenderer;
};


inline void Renderer2::renderSprite(const Sprite& sprite, PixPos leftTop) const
{
   m_spriteRenderer->render(sprite, leftTop);
}
