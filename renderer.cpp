//
// Nov-2020, Michael Lindner
// MIT license
//
#include "renderer.h"
#include "data.h"
#include "essentutils/filesys.h"
#include "glfw/glfw3.h"
#include "glm/gtx/rotate_vector.hpp"


bool Renderer::setup()
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


void Renderer::cleanup()
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


void Renderer::renderFrame()
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

   gll::VertexArray::unbind();
   gll::Texture2D::unbind();
}


bool Renderer::setupShaders()
{
   const std::filesystem::path appPath = sutil::appDirectory();
   bool ok = !appPath.empty();

   gll::Shader vs{gll::makeVertexShader(appPath / "towers.vs")};
   if (ok)
      ok = vs.compile();

   gll::Shader fs{gll::makeFragmentShader(appPath / "towers.fs")};
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


bool Renderer::setupTextures()
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


bool Renderer::setupData()
{
   m_vao.create();
   m_vao.bind();

   m_posBuf.create();
   m_posBuf.bind(GL_ARRAY_BUFFER);
   m_posBuf.setData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
   // The attribute index has to match the 'location' value in the vertex shader code.
   constexpr GLuint posAttribIdx = 0;
   m_vao.setAttribFormat(posAttribIdx, posFormat);
   m_vao.enableAttrib(posAttribIdx);

   m_normalBuf.create();
   m_normalBuf.bind(GL_ARRAY_BUFFER);
   m_normalBuf.setData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
   // The attribute index has to match the 'location' value in the vertex shader code.
   constexpr GLuint normalAttribIdx = 1;
   m_vao.setAttribFormat(normalAttribIdx, normalFormat);
   m_vao.enableAttrib(normalAttribIdx);

   m_colorBuf.create();
   m_colorBuf.bind(GL_ARRAY_BUFFER);
   m_colorBuf.setData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
   // The attribute index has to match the 'location' value in the vertex shader code.
   constexpr GLuint colorAttribIdx = 2;
   m_vao.setAttribFormat(colorAttribIdx, colorFormat);
   m_vao.enableAttrib(colorAttribIdx);

   m_texCoordBuf.create();
   m_texCoordBuf.bind(GL_ARRAY_BUFFER);
   m_texCoordBuf.setData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);
   // The attribute index has to match the 'location' value in the vertex shader code.
   constexpr GLuint texCoordAttribIdx = 3;
   m_vao.setAttribFormat(texCoordAttribIdx, texCoordFormat);
   m_vao.enableAttrib(texCoordAttribIdx);

   m_elemBuf.create();
   m_elemBuf.bind(GL_ELEMENT_ARRAY_BUFFER);
   m_elemBuf.setData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

   // Unbind vao first to stop "recording" information in it.
   m_vao.unbind();
   // Unbind each buffer type from global state.
   gll::Buffer::unbind(GL_ARRAY_BUFFER);
   gll::Buffer::unbind(GL_ELEMENT_ARRAY_BUFFER);
   gll::Texture2D::unbind();

   return true;
}


bool Renderer::setupRendering()
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


bool Renderer::setupLighting()
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
