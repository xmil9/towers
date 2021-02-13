//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "sge_mesh2.h"
#include "sge_types.h"
#include "glm/vec2.hpp"
#include "opengl_util/gll_vao.h"
#include <filesystem>
#include <cstddef>

namespace gll
{
class Program;
}
namespace sge
{
class Resources;
class Sprite;
}


namespace sge
{
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

   bool setup();
   void render(gll::Program& shaders, const Sprite& sprite, PixPos leftTop,
               const Color& tint = NoColor) const;

 private:
   bool setupData();
   void makeVao(const Mesh2& mesh);

 private:
   Resources* m_resources = nullptr;
   gll::Vao m_vao;
   std::size_t m_numElements = 0;
};


inline SpriteRenderer::SpriteRenderer(Resources* resources) : m_resources{resources}
{
}

} // namespace sge
