//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "mesh2.h"
#include "gll_binding.h"
#include "gll_vao.h"
#include "gll_vbo.h"
#include "glm/vec2.hpp"
#include <cstddef>

namespace gll
{
struct DataFormat;
class Program;
} // namespace gll
class Resources;
class SpriteLook;


///////////////////

class SpriteRenderer
{
 public:
   explicit SpriteRenderer(Resources* resources);
   ~SpriteRenderer() = default;
   SpriteRenderer(const SpriteRenderer&) = delete;
   SpriteRenderer(SpriteRenderer&&) = default;

   SpriteRenderer& operator=(const SpriteRenderer&) = delete;
   SpriteRenderer& operator=(SpriteRenderer&&) = default;

   void setMesh(const Mesh2& mesh);
   void render(const gll::Program& shaders, const SpriteLook& look, const glm::vec2& pos,
               const glm::vec2& size, float rot) const;

 private:
   // Combines a vbo and an object that controls the vbo's binding to th global
   // OpenGL state.
   struct BoundVbo
   {
      gll::Vbo vbo;
      gll::VboBinding binding;
   };

   void makeVao(const Mesh2& mesh);
   void bindArrayVbo(GLuint attribIdx, const void* data, std::size_t dataSize,
                     const gll::DataFormat& format, BoundVbo& buf);
   void bindElementVbo(const void* data, std::size_t dataSize, BoundVbo& buf);

 private:
   Resources* m_resources = nullptr;
   gll::Vao m_vao;
   std::size_t m_numElements = 0;
};


inline SpriteRenderer::SpriteRenderer(Resources* resources) : m_resources{resources}
{
}
