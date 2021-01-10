//
// Nov-2020, Michael Lindner
// MIT license
//
#include "sprite_renderer.h"
#include "resources.h"
#include "sprite.h"
#include "gll_binding.h"
#include "gll_data_format.h"
#include "gll_program.h"
#include "gll_shader.h"
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

bool SpriteRenderer::setup(const std::filesystem::path& shaderPath)
{
   return setupShaders(shaderPath) && setupData();
}


void SpriteRenderer::render(const Sprite& sprite, PixPos leftTop) const
{
   gll::BindingScope<gll::Texture2D> texBinding;
   if (sprite.hasTexture())
   {
      glActiveTexture(GL_TEXTURE0);
      texBinding.bind(m_resources->getTexture(sprite.texture()));
   }

   makeUniform("spriteColor", sprite.color());

   // Scope for vao binding.
   {
      gll::BindingScope vaoBinding{m_vao};

      gll::Uniform modelUf = m_shaders.uniform("model");
      modelUf.setValue(
         calcModelMatrix(leftTop, sprite.size(), sprite.rotation(), sprite.rotationCenter()));

      glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_numElements), GL_UNSIGNED_INT,
                     nullptr);
   }
}


bool SpriteRenderer::setupShaders(const std::filesystem::path& shaderPath)
{
   gll::Shader vs{gll::makeVertexShader(shaderPath / "sprite_shader.vs")};
   gll::Shader fs{gll::makeFragmentShader(shaderPath / "sprite_shader.fs")};
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


bool SpriteRenderer::setupData()
{
   // Coord system for vertex coordinates is:
   // (0, 0) - left-top, (1, 1) - right-bottom
   const std::vector<Mesh2::Point> positions = {
      {0.f, 1.f}, {1.f, 1.f}, {1.f, 0.f}, {0.f, 0.f}};
   // Triangle vertices are ordered ccw.
   const std::vector<Mesh2::VertexIdx> indices = {0, 1, 2, 2, 3, 0};
   // Coord system for texture coordinates is:
   // (0, 0) - left-bottom, (1, 1) - right-top
   const std::vector<Mesh2::Point> texCoords = positions;

   Mesh2 mesh;
   mesh.setPositions(positions);
   mesh.setIndices(indices);
   mesh.setTextureCoords(texCoords);

   m_numElements = mesh.numIndices();
   makeVao(mesh);

   return true;
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
