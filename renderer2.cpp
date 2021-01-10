//
// Nov-2020, Michael Lindner
// MIT license
//
#include "renderer2.h"
#include "resources.h"


bool Renderer2::setup(Resources* resources, int viewWidth, int viewHeight)
{
   assert(resources);

   m_resources = resources;
   m_spriteRenderer = std::make_unique<SpriteRenderer>(resources);

   return m_spriteRenderer->setup(m_resources->shaderPath()) &&
          setupSetting(viewWidth, viewHeight);
}


bool Renderer2::setupSetting(int viewWidth, int viewHeight)
{
   glViewport(0, 0, viewWidth, viewHeight);
   setFrustumSize(viewWidth, viewHeight);

   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glClearColor(0.8f, 0.8f, 0.8f, 1.0f);

   // Blending settings.
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   return true;
}


void Renderer2::beginRendering(bool clear) const
{
   if (clear)
      glClear(GL_COLOR_BUFFER_BIT);
   
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
