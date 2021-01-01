//
// Nov-2020, Michael Lindner
// MIT license
//
#include "renderer2.h"
#include "resources.h"
#include "gll_shader.h"


bool Renderer2::setup(Resources* resources, int viewWidth, int viewHeight)
{
   m_resources = resources;
   return setupShaders() && setupSetting(viewWidth, viewHeight);
}


bool Renderer2::setupShaders()
{
   const std::filesystem::path path = m_resources->shaderPath();

   gll::Shader vs{gll::makeVertexShader(path / "sprite_shader.vs")};
   gll::Shader fs{gll::makeFragmentShader(path / "sprite_shader.fs")};
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


bool Renderer2::setupSetting(int viewWidth, int viewHeight)
{
   glViewport(0, 0, viewWidth, viewHeight);
   setFrustumSize(viewWidth, viewHeight);

   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
   return true;
}


void Renderer2::beginRendering(bool clear) const
{
   if (clear)
      glClear(GL_COLOR_BUFFER_BIT);
   m_shaders.use();

   gll::Uniform viewUf = m_shaders.uniform("view");
   viewUf.setValue(m_cam.viewMatrix());
   gll::Uniform projUf = m_shaders.uniform("projection");
   projUf.setValue(m_frustum.projectionMatrix());
}
