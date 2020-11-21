//
// Nov-2020, Michael Lindner
// MIT license
//
#include "renderer3.h"
#include "cube3_data.h"
#include "essentutils/filesys.h"
#include "gll_binding.h"
#include "glfw/glfw3.h"
#include "glm/gtx/rotate_vector.hpp"


bool Renderer3::setup()
{
   if (!setupShaders())
      return false;
   if (!setupTextures())
      return false;
   if (!setupData())
      return false;
   if (!setupRendering())
      return false;
   if (!setupLighting())
      return false;
   return true;
}


void Renderer3::cleanup()
{
   m_vao.destroy();
   m_posBuf.destroy();
   m_normalBuf.destroy();
   m_colorBuf.destroy();
   m_texCoordBuf.destroy();
   m_tex.destroy();
   m_tex2.destroy();
   m_elemBuf.destroy();
}


void Renderer3::renderFrame()
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glActiveTexture(GL_TEXTURE0);
   m_tex.bind();
   glActiveTexture(GL_TEXTURE1);
   m_tex2.bind();

   m_prog.use();
   m_vao.bind();

   // Move the light source.
   const float tm = static_cast<float>(glfwGetTime());
   const glm::vec3 lightPos{5.f * std::cos(tm), -3.f, 5.f * std::sin(tm)};
   gll::Uniform lightPosUf = m_prog.uniform("lightPos");
   lightPosUf.setValue(lightPos);

   gll::Uniform viewPosUf = m_prog.uniform("viewPos");
   viewPosUf.setValue(m_cam.position());

   // Set coordinate transformation matrices.
   gll::Uniform modelUf = m_prog.uniform("model");
   modelUf.setValue(m_modelMat);
   gll::Uniform normalMatUf = m_prog.uniform("normalMat");
   normalMatUf.setValue(m_normalMat);
   gll::Uniform viewUf = m_prog.uniform("view");
   viewUf.setValue(m_cam.viewMatrix());
   gll::Uniform projUf = m_prog.uniform("projection");
   projUf.setValue(m_frustum.projectionMatrix());

   glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(std::size(indices)), GL_UNSIGNED_INT,
                  nullptr);

   gll::Vao::unbind();
   gll::Texture2D::unbind();
}


bool Renderer3::setupShaders()
{
   const std::filesystem::path appPath = sutil::appDirectory();
   bool ok = !appPath.empty();

   gll::Shader vs{gll::makeVertexShader(appPath / "cube3_shader.vs")};
   if (ok)
      ok = vs.compile();

   gll::Shader fs{gll::makeFragmentShader(appPath / "cube3_shader.fs")};
   if (ok)
      ok = fs.compile();

   if (ok)
      ok = m_prog.create();
   if (ok)
   {
      m_prog.attachShader(vs);
      m_prog.attachShader(fs);
      ok = m_prog.link();
   }

   if (!ok)
      m_prog.destroy();

   return ok;
}


bool Renderer3::setupTextures()
{
   const std::filesystem::path appPath = sutil::appDirectory();
   if (appPath.empty())
      return false;

   m_tex.create();
   m_tex.bind();
   m_tex.setWrapPolicy(GL_REPEAT, GL_REPEAT);
   m_tex.setScaleFiltering(GL_NEAREST, GL_NEAREST);
   m_tex.loadData(appPath / "directions.png", true, 0, GL_RGB, GL_RGBA, GL_UNSIGNED_BYTE);
   m_tex.generateMipmap();

   m_tex2.create();
   m_tex2.bind();
   m_tex2.setWrapPolicy(GL_REPEAT, GL_REPEAT);
   m_tex2.setScaleFiltering(GL_NEAREST, GL_NEAREST);
   m_tex2.loadData(appPath / "red_marble.png", true, 0, GL_RGB, GL_RGBA,
                   GL_UNSIGNED_BYTE);
   m_tex2.generateMipmap();

   m_prog.use();
   m_prog.setTextureUnit("texSampler", 0);
   m_prog.setTextureUnit("texSampler2", 1);

   return true;
}


bool Renderer3::setupData()
{
   // VBOs and their binding scopes. They have to be unbound after the vao.
   gll::BoundVbo posBuf;
   gll::BoundVbo normalBuf;
   gll::BoundVbo colorBuf;
   gll::BoundVbo texCoordBuf;
   gll::BoundVbo elemBuf;

   // Scope for vao binding.
   // Needs to be unbound before the vbos.
   {
      m_vao.create();
      gll::BindingScope vaoBinding{m_vao};

      // Each attribute index has to match the 'location' value in the vertex shader code.
      constexpr GLuint PosAttribIdx = 0;
      constexpr GLuint NormalAttribIdx = 1;
      constexpr GLuint ColorAttribIdx = 2;
      constexpr GLuint TexCoordsAttribIdx = 3;

      bindArrayVbo(PosAttribIdx, positions, sizeof(positions), posFormat, posBuf);
      bindArrayVbo(NormalAttribIdx, normals, sizeof(normals), normalFormat, normalBuf);
      bindArrayVbo(ColorAttribIdx, colors, sizeof(colors), colorFormat, colorBuf);
      bindArrayVbo(TexCoordsAttribIdx, texCoords, sizeof(texCoords), texCoordFormat,
                   texCoordBuf);
      bindElementVbo(indices, sizeof(indices), elemBuf);
   }

   return true;
}


bool Renderer3::setupRendering()
{
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
   glEnable(GL_DEPTH_TEST);

   m_modelMat = glm::mat4(1.0f);
   m_modelMat =
      glm::rotate(m_modelMat, glm::radians(-20.0f), glm::vec3(1.0f, 0.0f, 0.0f));
   m_normalMat = glm::mat3(glm::transpose(glm::inverse(m_modelMat)));

   return true;
}


bool Renderer3::setupLighting()
{
   const glm::vec3 lightColor{1.0f, 1.0f, 1.0f};
   constexpr float ambientIntensity = 0.1f;
   const glm::vec3 ambient = lightColor * ambientIntensity;

   m_prog.use();
   gll::Uniform lightColorUf = m_prog.uniform("lightColor");
   lightColorUf.setValue(lightColor);
   // Ambient light.
   gll::Uniform ambientUf = m_prog.uniform("ambient");
   ambientUf.setValue(ambient);

   return true;
}
