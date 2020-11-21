//
// Nov-2020, Michael Lindner
// MIT license
//
#include "sprite_renderer.h"
#include "resources.h"
#include "sprite_look.h"
#include "gll_data_format.h"
#include "gll_program.h"


namespace
{
///////////////////

glm::mat4x4 rotateAtCenter(const glm::mat4x4& m, const glm::vec2& size, float rot)
{
   constexpr glm::vec3 rotNormal{0.f, 0.f, 1.f};
   return
      // Translate back to original position.
      glm::translate(
         // Rotate
         glm::rotate(
            // Translate center to origin.
            glm::translate(m, glm::vec3(.5f * size.x, .5f * size.y, 0.f)), rot,
            rotNormal),
         glm::vec3(-.5f * size.x, -.5f * size.y, 0.f));
}


glm::mat4x4 calcModelMatrix(const glm::vec2& pos, const glm::vec2& size, float rot)
{
   return
      // Finally scale.
      glm::scale(
         // Then rotate.
         rotateAtCenter(
            // First translate.
            glm::translate(glm::mat4(1.f), glm::vec3(pos, 0.f)), size, rot),
         glm::vec3(size, 1.f));
}

} // namespace


///////////////////

void SpriteRenderer::setMesh(const Mesh2& mesh)
{
   m_numElements = mesh.numIndices();
   makeVao(mesh);
}


void SpriteRenderer::render(const gll::Program& shaders, const SpriteLook& look,
                            const glm::vec2& pos, const glm::vec2& size, float rot) const
{
   gll::Binding<gll::Texture2D> texBinding;
   if (look.hasTexture())
   {
      glActiveTexture(GL_TEXTURE0);
      texBinding.bind(m_resources->getTexture(look.texture()));
   }

   // Scope for VAO binding.
   {
      gll::Binding vaoBinding{m_vao};

      gll::Uniform modelUf = shaders.uniform("model");
      modelUf.setValue(calcModelMatrix(pos, size, rot));

      glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_numElements), GL_UNSIGNED_INT,
                     nullptr);
   }
}


void SpriteRenderer::makeVao(const Mesh2& mesh)
{
   // VBOs and their bindings. They have to be unbound after the VAO.
   BoundBuffer posBuf;
   BoundBuffer texCoordBuf;
   BoundBuffer elemBuf;

   // Scope for VAO binding.
   // Needs to be unbound before the VBOs.
   {
      m_vao.create();
      gll::Binding vaoBinding{m_vao};

      // Each attribute index has to match the 'location' value in the vertex shader code.
      constexpr GLuint posAttribIdx = 0;
      constexpr GLuint texCoordsAttribIdx = 1;
      
      makePositionVbo(mesh, posAttribIdx, posBuf);
      makeTextureCoordVbo(mesh, texCoordsAttribIdx, texCoordBuf);
      makeElementVbo(mesh, elemBuf);
   }
}


void SpriteRenderer::makePositionVbo(const Mesh2& mesh, GLuint attribIdx, BoundBuffer& buf)
{
   buf.vbo.create();
   buf.binding.bind(buf.vbo, GL_ARRAY_BUFFER);
   buf.vbo.setData(GL_ARRAY_BUFFER, mesh.numPositionBytes(), mesh.positions(),
                   GL_STATIC_DRAW);
   m_vao.setAttribFormat(attribIdx, mesh.positionsFormat());
   m_vao.enableAttrib(attribIdx);
}


void SpriteRenderer::makeTextureCoordVbo(const Mesh2& mesh, GLuint attribIdx,
                                         BoundBuffer& buf)
{
   if (mesh.numTextureCoords() > 0)
   {
      buf.vbo.create();
      buf.binding.bind(buf.vbo, GL_ARRAY_BUFFER);
      buf.vbo.setData(GL_ARRAY_BUFFER, mesh.numTextureCoordBytes(), mesh.textureCoords(),
                      GL_STATIC_DRAW);
      // The attribute index has to match the 'location' value in the vertex shader
      // code.
      m_vao.setAttribFormat(attribIdx, mesh.textureCoordsFormat());
      m_vao.enableAttrib(attribIdx);
   }
}


void SpriteRenderer::makeElementVbo(const Mesh2& mesh, BoundBuffer& buf)
{
   buf.vbo.create();
   buf.binding.bind(buf.vbo, GL_ELEMENT_ARRAY_BUFFER);
   buf.vbo.setData(GL_ELEMENT_ARRAY_BUFFER, mesh.numIndexBytes(), mesh.indices(),
                   GL_STATIC_DRAW);
}
