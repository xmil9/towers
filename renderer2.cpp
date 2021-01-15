//
// Nov-2020, Michael Lindner
// MIT license
//
#include "renderer2.h"
#include "resources.h"
#include "glm/gtc/matrix_transform.hpp"



bool Renderer2::setup(Resources* resources, int viewWidth, int viewHeight)
{
   assert(resources);

   m_resources = resources;
   m_spriteRenderer = std::make_unique<SpriteRenderer>(resources);
   m_textRenderer = std::make_unique<TextRenderer>(resources);

   const std::filesystem::path fontPath = m_resources->fontPath() / "arial.ttf";
   return m_spriteRenderer->setup(m_resources->shaderPath()) &&
          m_textRenderer->setup(fontPath, 20) && setupSettings(viewWidth, viewHeight);
}


void Renderer2::clearScene() const
{
   glClear(GL_COLOR_BUFFER_BIT);
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


void Renderer2::beginSpriteRendering() const
{
   m_spriteRenderer->activateShaders();
   m_spriteRenderer->makeUniform("view", m_cam.viewMatrix());
   m_spriteRenderer->makeUniform("projection", m_frustum.projectionMatrix());
}


void Renderer2::renderAnimation(Animation& anim, PixPos leftTop) const
{
   const auto sprite = anim.nextFrame();
   if (sprite)
      m_spriteRenderer->render(**sprite, leftTop);
}


void Renderer2::beginTextRendering() const
{
   m_textRenderer->activateShaders();
   m_textRenderer->makeUniform("view", m_cam.viewMatrix());
   m_textRenderer->makeUniform("projection", m_frustum.projectionMatrix());
}
