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

glm::mat4x4 rotateAtCenter(const glm::mat4x4& m, const RenderDim& size, Angle_t rot)
{
   constexpr glm::vec3 rotNormal{0.f, 0.f, 1.f};
   return
      // Translate back to original position.
      glm::translate(
         // Rotate
         glm::rotate(
            // Translate center to origin.
            glm::translate(m, glm::vec3(.5f * size.x, .5f * size.y, 0.f)), rot.radians(),
            rotNormal),
         glm::vec3(-.5f * size.x, -.5f * size.y, 0.f));
}


glm::mat4x4 calcModelMatrix(const RenderPos& pos, const RenderDim& size, Angle_t rot)
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
                            const SpriteForm& form) const
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
      modelUf.setValue(calcModelMatrix(form.m_pos, form.m_size, form.m_rot));

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
                   mesh.positionsFormat(), posBuf);
      bindArrayVbo(TexCoordsAttribIdx, mesh.textureCoords(), mesh.numTextureCoordBytes(),
                   mesh.textureCoordsFormat(), texCoordBuf);
      bindElementVbo(mesh.indices(), mesh.numIndexBytes(), elemBuf);
   }
}
