//
// Nov-2020, Michael Lindner
// MIT license
//
#include "renderer2.h"



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

   if (!m_shaders.create())
      return false;
   if (!m_shaders.link())
      return false;

   return true;
}


void Renderer2::render(const std::vector<Sprite>& sprites) const
{
   m_shaders.use();

   gll::Uniform viewUf = m_shaders.uniform("view");
   viewUf.setValue(m_cam.viewMatrix());
   gll::Uniform projUf = m_shaders.uniform("projection");
   projUf.setValue(m_frustum.projectionMatrix());

   for (auto& sprite : sprites)
      sprite.render(m_shaders);
}
