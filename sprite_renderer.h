//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "mesh2.h"
#include "gll_vertex_array.h"
#include "glm/vec2.hpp"
#include <cstddef>

namespace gll
{
class Program;
}
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
               const glm::vec2& scale) const;

 private:
   Resources* m_resources = nullptr;
   gll::VertexArray m_vao;
   std::size_t m_numElements = 0;
};


inline SpriteRenderer::SpriteRenderer(Resources* resources) : m_resources{resources}
{
}


// sprite renderer
//   shader program - always same
//   sprite voa - made from mesh
//   render(texture, pos, rot)
//
//
// sprite
//   model - behavior
//   sprite renderer (shared)
//   sprite look (shared)
