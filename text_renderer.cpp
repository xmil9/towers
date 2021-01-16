//
// Jan-2021, Michael Lindner
// MIT license
//
#include "text_renderer.h"
#include "mesh2.h"
#include "resources.h"
#include "gll_binding.h"
#include "gll_program.h"
#include "gll_shader.h"
#include "ft2build.h"
#include "freetype/freetype.h"
#include <array>


///////////////////

static constexpr std::size_t NumCharVertices = 6;


// Helper functionality for TreeType library.
namespace ft
{
///////////////////

struct Lib
{
   Lib();
   ~Lib();
   Lib(const Lib&) = delete;
   Lib(Lib&&) noexcept = default;

   Lib& operator=(const Lib&) = delete;
   Lib& operator=(Lib&&) noexcept = default;

   FT_Library handle = nullptr;
};


Lib::Lib()
{
   if (FT_Init_FreeType(&handle))
      throw std::runtime_error("Failed to initialize FreeType library.");
}

Lib::~Lib()
{
   FT_Done_FreeType(handle);
}


///////////////////

struct Face
{
   explicit Face(const Lib& ft, const std::filesystem::path& font);
   ~Face();
   Face(const Face&) = delete;
   Face(Face&&) noexcept = default;

   Face& operator=(const Face&) = delete;
   Face& operator=(Face&&) noexcept = default;

   FT_GlyphSlot glyph();
   const FT_GlyphSlot glyph() const;
   FT_Bitmap& bitmap() { return glyph()->bitmap; }
   const FT_Bitmap& bitmap() const { return glyph()->bitmap; }

   FT_Face handle = nullptr;
};


Face::Face(const Lib& ft, const std::filesystem::path& font)
{
   if (FT_New_Face(ft.handle, font.string().c_str(), 0, &handle))
      throw std::runtime_error("Failed to create FT_Face instance.");
}

Face::~Face()
{
   FT_Done_Face(handle);
}

FT_GlyphSlot Face::glyph()
{
   assert(handle);
   return handle->glyph;
}

const FT_GlyphSlot Face::glyph() const
{
   assert(handle);
   return handle->glyph;
}

} // namespace ft


///////////////////

void TextRenderer::render(gll::Program& shaders, const std::string& text, PixPos baseline,
                          float scale, glm::vec3 color)
{
   glActiveTexture(GL_TEXTURE0);
   m_vao.bind();

   constexpr glm::mat4 identity{1.f};
   shaders.setUniform("model", identity);
   shaders.setUniform("spriteColor", color);
   shaders.setUniform("isText", true);

   // Render each character of given text.
   float x = baseline.x;
   for (char c : text)
   {
      const Char& ch = m_chars[c];

      const float xpos = x + ch.bearing.x * scale;
      const float decend = (ch.size.y - ch.bearing.y) * scale;
      const float ypos = baseline.y + decend;

      const float w = ch.size.x * scale;
      const float h = ch.size.y * scale;

      const std::array<Mesh2::Point, NumCharVertices> positions{
         Mesh2::Point{xpos, ypos - h}, Mesh2::Point{xpos, ypos},
         Mesh2::Point{xpos + w, ypos}, Mesh2::Point{xpos, ypos - h},
         Mesh2::Point{xpos + w, ypos}, Mesh2::Point{xpos + w, ypos - h}};

      ch.tex.bind();

      m_posVbo.bind(GL_ARRAY_BUFFER);
      m_posVbo.setSubData(GL_ARRAY_BUFFER, 0, sizeof(Mesh2::Point) * positions.size(),
                          positions.data());
      m_posVbo.unbind(GL_ARRAY_BUFFER);

      glDrawArrays(GL_TRIANGLES, 0, NumCharVertices);

      x += ch.advanceX * scale;
   }

   m_vao.unbind();
   gll::Texture2D::unbind();
}


PixDim TextRenderer::measure(const std::string& text, float scale) const
{
   PixDim dim{0.f, 0.f};
   for (char c : text)
   {
      const auto pos = m_chars.find(c);
      if (pos != m_chars.end())
         dim += PixDim{pos->second.size + pos->second.bearing};
   }
   return dim * scale;
}


bool TextRenderer::setupCharTextures(const std::filesystem::path& font,
                                     unsigned int fontSize)
{
   try
   {
      ft::Lib ft;
      ft::Face face(ft, font);

      // set size to load glyphs as
      FT_Set_Pixel_Sizes(face.handle, 0, fontSize);

      // Align at byte border. Basically no alignment.
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

      // Load first 128 ascii characters.
      for (unsigned char c = 0; c < 128; c++)
      {
         if (FT_Load_Char(face.handle, c, FT_LOAD_RENDER))
            continue;

         gll::Texture2D tex;
         if (!tex.create())
            continue;
         tex.bind();
         tex.setData(0, GL_RED, face.bitmap().width, face.bitmap().rows, GL_RED,
                     GL_UNSIGNED_BYTE, face.bitmap().buffer);
         tex.setWrapPolicy(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
         tex.setScaleFiltering(GL_LINEAR, GL_LINEAR);

         Char ch;
         ch.tex = std::move(tex);
         ch.size = glm::ivec2{face.bitmap().width, face.bitmap().rows};
         ch.bearing = glm::ivec2{face.glyph()->bitmap_left, face.glyph()->bitmap_top};
         // The unit for the 'advance' vector is 1/64 of a pixel, so if the horizontal
         // advance distance is 4 pixel then advance.x = 256. Divide by 64 to get pixels.
         ch.advanceX = static_cast<unsigned int>(face.glyph()->advance.x) / 64;
         m_chars[c] = std::move(ch);
      }

      gll::Texture2D::unbind();
      return true;
   }
   catch (...)
   {
      return false;
   }
}

bool TextRenderer::setupBuffers()
{
   m_vao.create();
   gll::BindingScope vaoScope{m_vao};

   // Each attribute index has to match the 'location' value in the vertex shader code.
   constexpr GLuint PosAttribIdx = 0;
   constexpr GLuint TexCoordsAttribIdx = 1;

   // Same data format for position and texture coordinates.
   const gll::DataFormat VertexFormat{2, GL_FLOAT, GL_FALSE, sizeof(Mesh2::Point),
                                      nullptr};

   m_posVbo.create();
   bindArrayVbo(m_posVbo, PosAttribIdx, nullptr, NumCharVertices * sizeof(Mesh2::Point),
                VertexFormat, GL_DYNAMIC_DRAW, gll::Unbind::Immediately);

   // Texture coordinates are the same for all chars and never change.
   static const std::array<Mesh2::Point, NumCharVertices> CharTextureCoords{
      Mesh2::Point{0.f, 0.f}, Mesh2::Point{0.f, 1.f}, Mesh2::Point{1.f, 1.f},
      Mesh2::Point{0.f, 0.f}, Mesh2::Point{1.f, 1.f}, Mesh2::Point{1.f, 0.f}};
   m_texCoordVbo.create();
   bindArrayVbo(m_texCoordVbo, TexCoordsAttribIdx, CharTextureCoords.data(),
                NumCharVertices * sizeof(Mesh2::Point), VertexFormat, GL_DYNAMIC_DRAW,
                gll::Unbind::Immediately);

   return true;
}
