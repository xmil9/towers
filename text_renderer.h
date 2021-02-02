//
// Jan-2021, Michael Lindner
// MIT license
//
#pragma once
#include "basic_types.h"
#include "coords.h"
#include "gll_texture.h"
#include "gll_vao.h"
#include "gll_vbo.h"
#include "glm/vec3.hpp"
#include <cassert>
#include <filesystem>
#include <unordered_map>

namespace gll
{
class Program;
}
class Resources;


///////////////////

class TextRenderer
{
 public:
   explicit TextRenderer(Resources* resources);
   ~TextRenderer() = default;
   TextRenderer(const TextRenderer&) = delete;
   TextRenderer(TextRenderer&&) noexcept = default;

   TextRenderer& operator=(const TextRenderer&) = delete;
   TextRenderer& operator=(TextRenderer&&) noexcept = default;

   bool setup(const std::filesystem::path& font, unsigned int fontSize);
   void render(gll::Program& shaders, const std::string& text, PixPos baseline,
               float scale, const Color& color);
   PixDim measure(const std::string& text, float scale) const;

 private:
   // Information about how to draw each character.
   struct Char
   {
      // Character texture.
      gll::Texture2D tex;
      // Chacter size.
      glm::ivec2 size{0, 0};
      // Offset from origin to left-top of character.
      glm::ivec2 bearing{0, 0};
      // Horz offset to to next character.
      unsigned int advanceX = 0;
   };

 private:
   bool setupCharTextures(const std::filesystem::path& font, unsigned int fontSize);
   bool setupBuffers();

 private:
   Resources* m_resources = nullptr;
   std::unordered_map<GLchar, Char> m_chars;
   gll::Vao m_vao;
   gll::Vbo m_posVbo;
   gll::Vbo m_texCoordVbo;
};


inline TextRenderer::TextRenderer(Resources* resources) : m_resources{resources}
{
   assert(m_resources);
}

inline bool TextRenderer::setup(const std::filesystem::path& font, unsigned int fontSize)
{
   return setupCharTextures(font, fontSize) && setupBuffers();
}
