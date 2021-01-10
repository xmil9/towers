//
// Jan-2021, Michael Lindner
// MIT license
//
#pragma once
#include "coords.h"
#include "gll_program.h"
#include "gll_texture.h"
#include "gll_vao.h"
#include "gll_vbo.h"
#include "glm/vec3.hpp"
#include <cassert>
#include <filesystem>
#include <map>

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

   // Initial setup.
   bool setup(const std::filesystem::path& font, unsigned int fontSize);

   // Prepare a render session.
   void activateShaders() { m_shaders.use(); }
   template<typename Value>
   void makeUniform(const GLchar* name, const Value& v) const;
   
   // Individual render operations.
   void render(const std::string& text, PixPos pos, float scale, glm::vec3 color);

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
   bool setupShaders();
   bool setupCharTextures(const std::filesystem::path& font, unsigned int fontSize);
   bool setupVao();

 private:
   Resources* m_resources = nullptr;
   std::map<GLchar, Char> m_chars;
   gll::Program m_shaders;
   gll::Vao m_vao;
   gll::Vbo m_vbo;
};


inline TextRenderer::TextRenderer(Resources* resources) : m_resources{resources}
{
   assert(m_resources);
}

inline bool TextRenderer::setup(const std::filesystem::path& font, unsigned int fontSize)
{
   return setupCharTextures(font, fontSize) && setupShaders() && setupVao();
}

template<typename Value>
void TextRenderer::makeUniform(const GLchar* name, const Value& v) const
{
   gll::Uniform u = m_shaders.uniform(name);
   u.setValue(v);
}
