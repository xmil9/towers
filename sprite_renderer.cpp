//
// Nov-2020, Michael Lindner
// MIT license
//
#include "sprite_renderer.h"
#include "resources.h"
#include "sge_sprite.h"
#include "opengl_util/gll_binding.h"
#include "opengl_util/gll_data_format.h"
#include "opengl_util/gll_program.h"
#include "opengl_util/gll_shader.h"
#include "opengl_util/gll_vbo.h"


namespace
{
///////////////////

glm::mat4x4 rotateAround(const glm::mat4x4& m, sge::Angle_t rot,
                         const sge::PixPos& rotCenter)
{
   constexpr glm::vec3 rotNormal{0.f, 0.f, 1.f};
   return
      // Translate back to original position.
      glm::translate(
         // Rotate
         glm::rotate(
            // Translate to center of rotation.
            glm::translate(m, glm::vec3(rotCenter.x, rotCenter.y, 0.f)), rot.radians(),
            rotNormal),
         glm::vec3(-rotCenter.x, -rotCenter.y, 0.f));
}


glm::mat4x4 calcModelMatrix(const sge::PixPos& pos, const sge::PixDim& size,
                            sge::Angle_t rot, const sge::PixPos& rotCenter)
{
   return
      // Finally scale.
      glm::scale(
         // Then rotate.
         rotateAround(
            // First translate.
            glm::translate(glm::mat4(1.f), glm::vec3(pos, 0.f)), rot, rotCenter),
         glm::vec3(size, 1.f));
}

} // namespace


///////////////////

bool SpriteRenderer::setup()
{
   return setupData();
}


void SpriteRenderer::render(gll::Program& shaders, const sge::Sprite& sprite,
                            sge::PixPos leftTop, const sge::Color& tint) const
{
   gll::BindingScope<gll::Texture2D> texBinding;
   if (sprite.hasTexture())
   {
      glActiveTexture(GL_TEXTURE0);
      texBinding.bind(m_resources->getTexture(sprite.texture()));
   }

   shaders.setUniform("spriteColor", tint);
   shaders.setUniform("isText", false);

   // Scope for vao binding.
   {
      gll::BindingScope vaoBinding{m_vao};

      shaders.setUniform("model",
                         calcModelMatrix(leftTop, sprite.size(), sprite.rotation(),
                                         sprite.rotationCenter()));
      glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_numElements), GL_UNSIGNED_INT,
                     nullptr);
   }
}


bool SpriteRenderer::setupData()
{
   // Coord system for vertex coordinates is:
   // (0, 0) - left-top, (1, 1) - right-bottom
   const std::vector<sge::Mesh2::Point> positions = {
      {0.f, 1.f}, {1.f, 1.f}, {1.f, 0.f}, {0.f, 0.f}};
   // Triangle vertices are ordered ccw.
   const std::vector<sge::Mesh2::VertexIdx> indices = {0, 1, 2, 2, 3, 0};
   // Coord system for texture coordinates is:
   // (0, 0) - left-bottom, (1, 1) - right-top
   const std::vector<sge::Mesh2::Point> texCoords = positions;

   sge::Mesh2 mesh;
   mesh.setPositions(positions);
   mesh.setIndices(indices);
   mesh.setTextureCoords(texCoords);

   m_numElements = mesh.numIndices();
   makeVao(mesh);

   return true;
}


void SpriteRenderer::makeVao(const sge::Mesh2& mesh)
{
   m_vao.create();
   m_vao.bind();

   // Each attribute index has to match the 'location' value in the vertex shader code.
   constexpr GLuint PosAttribIdx = 0;
   constexpr GLuint TexCoordsAttribIdx = 1;

   gll::Vbo posVbo;
   posVbo.create();
   bindArrayVbo(posVbo, PosAttribIdx, mesh.positions(), mesh.numPositionBytes(),
                mesh.positionsFormat(), GL_STATIC_DRAW, gll::Unbind::LeaveBound);

   gll::Vbo texCoordVbo;
   texCoordVbo.create();
   bindArrayVbo(texCoordVbo, TexCoordsAttribIdx, mesh.textureCoords(),
                mesh.numTextureCoordBytes(), mesh.textureCoordsFormat(), GL_STATIC_DRAW,
                gll::Unbind::LeaveBound);

   gll::Vbo elemVbo;
   elemVbo.create();
   bindElementVbo(elemVbo, mesh.indices(), mesh.numIndexBytes(), GL_STATIC_DRAW,
                  gll::Unbind::LeaveBound);

   // Unbind before the vbos.
   m_vao.unbind();
   // Unbind vbos after vao.
   posVbo.unbind(GL_ARRAY_BUFFER);
   texCoordVbo.unbind(GL_ARRAY_BUFFER);
   elemVbo.unbind(GL_ELEMENT_ARRAY_BUFFER);
}
