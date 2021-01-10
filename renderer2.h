//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "camera_2d.h"
#include "frustum2.h"
#include "sprite_renderer.h"
#include "text_renderer.h"
#include "glm/vec3.hpp"

class Animation;
class Resources;
class Sprite;


///////////////////

class Renderer2
{
 public:
   bool setup(Resources* resources, int viewWidth, int viewHeight);
   void setFrustumSize(int width, int height) { m_frustum.setSize(width, height); }

   void clearScene() const;
   
   void beginSpriteRendering() const;
   void renderSprite(const Sprite& sprite, PixPos leftTop) const;

   void beginTextRendering() const;
   void renderAnimation(Animation& anim, PixPos leftTop) const;
   void renderText(const std::string& text, PixPos pos, float scale,
                   const glm::vec3& color);

 private:
   bool setupSettings(int viewWidth, int viewHeight);

 private:
   Resources* m_resources = nullptr;
   Camera2d m_cam;
   Frustum2 m_frustum;
   std::unique_ptr<SpriteRenderer> m_spriteRenderer;
   std::unique_ptr<TextRenderer> m_textRenderer;
};


inline void Renderer2::renderSprite(const Sprite& sprite, PixPos leftTop) const
{
   m_spriteRenderer->render(sprite, leftTop);
}

inline void Renderer2::renderText(const std::string& text, PixPos pos, float scale,
                                  const glm::vec3& color)
{
   m_textRenderer->render(text, pos, scale, color);
}
