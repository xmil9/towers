//
// Jan-2021, Michael Lindner
// MIT license
//
#include "text_renderer.h"
#include "resources.h"
#include "gll_binding.h"
#include "gll_shader.h"
#include "ft2build.h"
#include "freetype/freetype.h"


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

void TextRenderer::render(const std::string& text, PixPos pos, float scale,
                          glm::vec3 color)
{
   m_shaders.use();
   makeUniform("textColor", color);

   glActiveTexture(GL_TEXTURE0);
   m_vao.bind();

   // Render each character in given text.
   float x = pos.x;
   for (char c : text)
   {
      const Char& ch = m_chars[c];

      const float xpos = x + ch.bearing.x * scale;
      const float ypos = pos.y - (ch.size.y - ch.bearing.y) * scale;

      const float w = ch.size.x * scale;
      const float h = ch.size.y * scale;

      float vertices[6][4] = {
         {xpos, ypos + h, 0.0f, 0.0f},    {xpos, ypos, 0.0f, 1.0f},
         {xpos + w, ypos, 1.0f, 1.0f},

         {xpos, ypos + h, 0.0f, 0.0f},    {xpos + w, ypos, 1.0f, 1.0f},
         {xpos + w, ypos + h, 1.0f, 0.0f}};

      ch.tex.bind();
      m_vbo.bind(GL_ARRAY_BUFFER);
      m_vbo.setSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
      m_vbo.unbind(GL_ARRAY_BUFFER);

      glDrawArrays(GL_TRIANGLES, 0, 6);

      // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
      // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of
      // 1/64th pixels by 64 to get amount of pixels))
      x += (ch.advanceX >> 6) * scale;
   }

   m_vao.unbind();
   gll::Texture2D::unbind();
}


bool TextRenderer::setupShaders()
{
   const std::filesystem::path path = m_resources->shaderPath();

   gll::Shader vs{gll::makeVertexShader(path / "text_shader.vs")};
   gll::Shader fs{gll::makeFragmentShader(path / "text_shader.fs")};
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
         ch.advanceX = static_cast<unsigned int>(face.glyph()->advance.x);
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
   m_vao.bind();

   m_vbo.create();
   m_vbo.bind(GL_ARRAY_BUFFER);
   m_vbo.setData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);

   // Each attribute index has to match the 'location' value in the vertex shader code.
   constexpr GLuint PosAttribIdx = 0;

   gll::Vao::enableAttrib(PosAttribIdx);
   gll::Vao::setAttribFormat(PosAttribIdx, {4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr});

   m_vao.unbind();
   m_vbo.unbind(GL_ARRAY_BUFFER);

   return true;
}
