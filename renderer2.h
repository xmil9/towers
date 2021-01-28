//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "basic_types.h"
#include "camera_2d.h"
#include "frustum2.h"
#include "sprite.h"
#include "sprite_renderer.h"
#include "text_renderer.h"
#include "gll_program.h"
#include "gll_uniform.h"
#include "glm/vec3.hpp"

class Animation;
class Resources;


///////////////////

class Renderer2
{
 public:
   bool setup(Resources* resources, int viewWidth, int viewHeight);
   void setFrustumSize(int width, int height) { m_frustum.setSize(width, height); }

   void clearScene() const;
   void beginRendering();
   void renderSprite(const Sprite& sprite, PixPos leftTop, const Color& tint = NoColor);
   void renderSpriteCentered(const Sprite& sprite, PixPos center,
                             const Color& tint = NoColor);
   void renderAnimation(Animation& anim, PixPos leftTop);
   void renderAnimationCentered(Animation& anim, PixPos center);
   void renderText(const std::string& text, PixPos pos, float scale, const Color& color);
   PixDim measureText(const std::string& text, float scale) const;


 private:
   bool setupShaders(const std::filesystem::path& shaderPath);
   bool setupSettings(int viewWidth, int viewHeight);

 private:
   Resources* m_resources = nullptr;
   Camera2d m_cam;
   Frustum2 m_frustum;
   gll::Program m_shaders;
   std::unique_ptr<SpriteRenderer> m_spriteRenderer;
   std::unique_ptr<TextRenderer> m_textRenderer;
};


inline void Renderer2::renderSprite(const Sprite& sprite, PixPos leftTop,
                                    const Color& tint)
{
   m_spriteRenderer->render(m_shaders, sprite, leftTop, tint);
}

inline void Renderer2::renderSpriteCentered(const Sprite& sprite, PixPos center,
                                            const Color& tint)
{
   renderSprite(sprite, center - .5f * sprite.size(), tint);
}

inline void Renderer2::renderText(const std::string& text, PixPos pos, float scale,
                                  const Color& color)
{
   m_textRenderer->render(m_shaders, text, pos, scale, color);
}

inline PixDim Renderer2::measureText(const std::string& text, float scale) const
{
   return m_textRenderer->measure(text, scale);
}
