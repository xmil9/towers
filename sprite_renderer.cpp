//
// Nov-2020, Michael Lindner
// MIT license
//
#include "sprite_renderer.h"
#include "resources.h"
#include "sprite_look.h"
#include "gll_buffer.h"
#include "gll_data_format.h"
#include "gll_program.h"


namespace
{
///////////////////

glm::mat4x4 rotateAtCenter(const glm::mat4x4& m, const glm::vec2& size, float rot)
{
   constexpr glm::vec3 rotNormal{0.f, 0.f, 1.f};
   return glm::translate(
      glm::rotate(glm::translate(m, glm::vec3(.5f * size.x, .5f * size.y, 0.f)), rot,
                  rotNormal),
      glm::vec3(-.5f * size.x, -.5f * size.y, 0.f));
}


glm::mat4x4 calcModelMatrix(const glm::vec2& pos, const glm::vec2& size, float rot)
{
   return glm::scale(
      rotateAtCenter(glm::translate(glm::mat4(1.f), glm::vec3(pos, 0.f)), size, rot),
      glm::vec3(size, 1.f));
}

} // namespace


///////////////////

void SpriteRenderer::setMesh(const Mesh2& mesh)
{
   m_numElements = mesh.numIndices();

   m_vao.create();
   m_vao.bind();

   gll::Buffer posBuf;
   posBuf.create();
   posBuf.bind(GL_ARRAY_BUFFER);
   posBuf.setData(GL_ARRAY_BUFFER, mesh.numPositionBytes(), mesh.positions(),
                  GL_STATIC_DRAW);
   // The attribute index has to match the 'location' value in the vertex shader code.
   constexpr GLuint posAttribIdx = 0;
   constexpr gll::DataFormat posFormat = {2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                                          nullptr};
   m_vao.setAttribFormat(posAttribIdx, posFormat);
   m_vao.enableAttrib(posAttribIdx);

   gll::Buffer texCoordBuf;
   if (mesh.numTextureCoords() > 0)
   {
      texCoordBuf.create();
      texCoordBuf.bind(GL_ARRAY_BUFFER);
      texCoordBuf.setData(GL_ARRAY_BUFFER, mesh.numTextureCoordBytes(),
                          mesh.textureCoords(), GL_STATIC_DRAW);
      // The attribute index has to match the 'location' value in the vertex shader code.
      constexpr GLuint texCoordsAttribIdx = 1;
      constexpr gll::DataFormat texCoordFormat = {2, GL_FLOAT, GL_FALSE,
                                                  2 * sizeof(float), nullptr};
      m_vao.setAttribFormat(texCoordsAttribIdx, texCoordFormat);
      m_vao.enableAttrib(texCoordsAttribIdx);
   }

   gll::Buffer elemBuf;
   elemBuf.create();
   elemBuf.bind(GL_ELEMENT_ARRAY_BUFFER);
   elemBuf.setData(GL_ELEMENT_ARRAY_BUFFER, mesh.numIndexBytes(), mesh.indices(),
                   GL_STATIC_DRAW);

   // Unbind vao first to stop "recording" information in it.
   m_vao.unbind();
   // Unbind each buffer type from global state.
   gll::Buffer::unbind(GL_ARRAY_BUFFER);
   gll::Buffer::unbind(GL_ELEMENT_ARRAY_BUFFER);
}


void SpriteRenderer::render(const gll::Program& shaders, const SpriteLook& look,
                            const glm::vec2& pos, const glm::vec2& size, float rot) const
{
   if (look.hasTexture())
   {
      glActiveTexture(GL_TEXTURE0);
      const gll::Texture2D& tex = m_resources->getTexture(look.texture());
      tex.bind();
   }

   m_vao.bind();

   gll::Uniform modelUf = shaders.uniform("model");
   modelUf.setValue(calcModelMatrix(pos, size, rot));

   glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_numElements), GL_UNSIGNED_INT,
                  nullptr);

   gll::VertexArray::unbind();
   if (look.hasTexture())
      gll::Texture2D::unbind();
}
