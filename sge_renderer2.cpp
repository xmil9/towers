//
// Nov-2020, Michael Lindner
// MIT license
//
#include "sge_animation.h"
#include "sge_renderer2.h"
#include "glm/gtc/matrix_transform.hpp"
#include "opengl_util/gll_shader.h"


namespace sge
{
///////////////////

bool Renderer2::setup(Resources* resources, const std::filesystem::path& shaderDir,
                      const std::filesystem::path& fontDir, int viewWidth,
                      int viewHeight)
{
   assert(resources);

   m_spriteRenderer = std::make_unique<SpriteRenderer>(resources);
   m_textRenderer = std::make_unique<TextRenderer>(resources);

   const std::filesystem::path fontPath = fontDir / "arial.ttf";
   constexpr unsigned int fontSize = 20;

   return setupShaders(shaderDir) && m_spriteRenderer->setup() &&
          m_textRenderer->setup(fontPath, fontSize) &&
          setupSettings(viewWidth, viewHeight);
}


void Renderer2::clearScene() const
{
   glClear(GL_COLOR_BUFFER_BIT);
}


void Renderer2::beginRendering()
{
   m_shaders.use();
   m_shaders.setUniform("view", m_cam.viewMatrix());
   m_shaders.setUniform("projection", m_frustum.projectionMatrix());
}


void Renderer2::renderAnimation(Animation& anim, PixPos leftTop, bool advanceFrame)
{
   const auto sprite = advanceFrame ? anim.nextFrame() : anim.currentFrame();
   if (sprite)
      m_spriteRenderer->render(m_shaders, **sprite, leftTop);
}


void Renderer2::renderAnimationCentered(Animation& anim, PixPos center, bool advanceFrame)
{
   const auto sprite = advanceFrame ? anim.nextFrame() : anim.currentFrame();
   if (sprite)
   {
      const PixPos leftTop = center - .5f * (*sprite)->size();
      m_spriteRenderer->render(m_shaders, **sprite, leftTop);
   }
}


bool Renderer2::setupShaders(const std::filesystem::path& shaderPath)
{
   gll::Shader vs{gll::makeVertexShader(shaderPath / "sge_sprite_shader.vs")};
   gll::Shader fs{gll::makeFragmentShader(shaderPath / "sge_sprite_shader.fs")};
   if (!vs || !fs)
      return false;

   if (!vs.compile() || !fs.compile())
      return false;

   if (!m_shaders.create())
      return false;

   m_shaders.attachShader(vs);
   m_shaders.attachShader(fs);

   if (!m_shaders.link())
      return false;

   return true;
}


bool Renderer2::setupSettings(int viewWidth, int viewHeight)
{
   glViewport(0, 0, viewWidth, viewHeight);
   setFrustumSize(viewWidth, viewHeight);

   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glClearColor(1.f, 1.f, 1.f, 1.0f);

   // Blending settings.
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   return true;
}

} // namespace sge
