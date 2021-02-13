//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "sge_camera_2d.h"
#include "sge_frustum2.h"
#include "sge_sprite.h"
#include "sge_sprite_renderer.h"
#include "sge_text_renderer.h"
#include "sge_types.h"
#include "glm/vec3.hpp"
#include "opengl_util/gll_program.h"
#include "opengl_util/gll_uniform.h"
#include <filesystem>

namespace sge
{
class Animation;
class Resources;
}


namespace sge
{
///////////////////

class Renderer2
{
 public:
   bool setup(Resources* resources, const std::filesystem::path& shaderDir,
              const std::filesystem::path& fontDir, int viewWidth, int viewHeight);
   void setFrustumSize(int width, int height) { m_frustum.setSize(width, height); }

   void clearScene() const;
   void beginRendering();
   void renderSprite(const Sprite& sprite, PixPos leftTop, const Color& tint = NoColor);
   void renderSpriteCentered(const Sprite& sprite, PixPos center,
                             const Color& tint = NoColor);
   void renderAnimation(Animation& anim, PixPos leftTop, bool advanceFrame = true);
   void renderAnimationCentered(Animation& anim, PixPos center, bool advanceFrame = true);
   void renderText(const std::string& text, PixPos pos, float scale, const Color& color);
   PixDim measureText(const std::string& text, float scale) const;


 private:
   bool setupShaders(const std::filesystem::path& shaderPath);
   bool setupSettings(int viewWidth, int viewHeight);

 private:
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

} // namespace sge
