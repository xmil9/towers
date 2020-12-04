//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "glad/glad.h" // glad must be included before anything else opengl related.
#include <filesystem>
#include <utility>


class PngTexture
{
 public:
   PngTexture() = default;
   explicit PngTexture(const std::filesystem::path& texFile, bool flipVert = false);
   ~PngTexture();
   PngTexture(const PngTexture&) = delete;
   PngTexture(PngTexture&& other);

   PngTexture& operator=(const PngTexture&) = delete;
   PngTexture& operator=(PngTexture&& other);
   explicit operator bool() const { return m_data != nullptr; }
   bool operator!() const { return !operator bool(); }

   bool load(const std::filesystem::path& texFile, bool flipVert = false);
   void release();
   int width() const { return m_width; }
   int height() const { return m_height; }
   const unsigned char* data() const { return m_data; }
   GLint format() const { return GL_RGBA; }
   GLenum internalFormat() const { return GL_RGBA; }
   GLenum pixelType() const { return GL_UNSIGNED_BYTE; }

   friend inline void swap(PngTexture& a, PngTexture& b)
   {
      std::swap(a.m_width, b.m_width);
      std::swap(a.m_height, b.m_height);
      std::swap(a.m_data, b.m_data);
   }

 private:
   void clear();

 private:
   int m_width = 0;
   int m_height = 0;
   unsigned char* m_data = nullptr;
};
