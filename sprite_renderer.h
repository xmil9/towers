//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "mesh2.h"
#include "gll_program.h"
#include "gll_vao.h"
#include "glm/vec2.hpp"
#include <filesystem>
#include <cstddef>

class Resources;
class Sprite;


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

   // Initial setup.
   bool setup(const std::filesystem::path& shaderPath);
   
   // Prepare a render session.
   void activateShaders() { m_shaders.use(); }
   template<typename Value>
   void makeUniform(const GLchar* name, const Value& v) const;
   
   // Individual render operations.
   void render(const Sprite& sprite, PixPos leftTop) const;

 private:
   bool setupShaders(const std::filesystem::path& shaderPath);
   bool setupData();
   void makeVao(const Mesh2& mesh);

 private:
   Resources* m_resources = nullptr;
   gll::Program m_shaders;
   gll::Vao m_vao;
   std::size_t m_numElements = 0;
};


inline SpriteRenderer::SpriteRenderer(Resources* resources) : m_resources{resources}
{
}

template<typename Value>
void SpriteRenderer::makeUniform(const GLchar* name, const Value& v) const
{
   gll::Uniform u = m_shaders.uniform(name);
   u.setValue(v);
}
