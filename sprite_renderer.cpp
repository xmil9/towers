//
// Nov-2020, Michael Lindner
// MIT license
//
#include "sprite_renderer.h"
#include "resources.h"
#include "sprite_form.h"
#include "sprite_look.h"
#include "gll_binding.h"
#include "gll_data_format.h"
#include "gll_program.h"
#include "gll_vbo.h"


namespace
{
///////////////////

glm::mat4x4 rotateAround(const glm::mat4x4& m, Angle_t rot, const PixPos& rotCenter)
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


glm::mat4x4 calcModelMatrix(const PixPos& pos, const PixDim& size, Angle_t rot,
                            const PixPos& rotCenter)
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

void SpriteRenderer::setMesh(const Mesh2& mesh)
{
   m_numElements = mesh.numIndices();
   makeVao(mesh);
}


void SpriteRenderer::render(const gll::Program& shaders, const SpriteLook& look,
                            const SpriteForm& form, PixPos leftTop) const
{
   gll::BindingScope<gll::Texture2D> texBinding;
   if (look.hasTexture())
   {
      glActiveTexture(GL_TEXTURE0);
      texBinding.bind(m_resources->getTexture(look.texture()));
   }

   gll::Uniform colorUf = shaders.uniform("spriteColor");
   colorUf.setValue(look.color());

   // Scope for vao binding.
   {
      gll::BindingScope vaoBinding{m_vao};

      gll::Uniform modelUf = shaders.uniform("model");
      modelUf.setValue(
         calcModelMatrix(leftTop, form.size(), form.rotation(), form.rotationCenter()));

      glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_numElements), GL_UNSIGNED_INT,
                     nullptr);
   }
}


void SpriteRenderer::makeVao(const Mesh2& mesh)
{
   // VBOs and their binding scopes. They have to be unbound after the vao.
   gll::BoundVbo posBuf;
   gll::BoundVbo texCoordBuf;
   gll::BoundVbo elemBuf;

   // Scope for vao binding.
   // Needs to be unbound before the vbos.
   {
      m_vao.create();
      gll::BindingScope vaoBinding{m_vao};

      // Each attribute index has to match the 'location' value in the vertex shader code.
      constexpr GLuint PosAttribIdx = 0;
      constexpr GLuint TexCoordsAttribIdx = 1;

      bindArrayVbo(PosAttribIdx, mesh.positions(), mesh.numPositionBytes(),
                   mesh.positionsFormat(), GL_STATIC_DRAW, posBuf);
      bindArrayVbo(TexCoordsAttribIdx, mesh.textureCoords(), mesh.numTextureCoordBytes(),
                   mesh.textureCoordsFormat(), GL_STATIC_DRAW, texCoordBuf);
      bindElementVbo(mesh.indices(), mesh.numIndexBytes(), GL_STATIC_DRAW, elemBuf);
   }
}
