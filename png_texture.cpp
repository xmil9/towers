//
// Nov-2020, Michael Lindner
// MIT license
//
#include "png_texture.h"
#include "stb/stb_image.h"


PngTexture::PngTexture(const std::filesystem::path& texFile, bool flipVert)
{
   load(texFile, flipVert);
}


PngTexture::~PngTexture()
{
   release();
}


PngTexture::PngTexture(PngTexture&& other)
   : m_width{other.m_width}, m_height{other.m_height}, m_data{other.m_data}
{
   other.clear();
}


PngTexture& PngTexture::operator=(PngTexture&& other)
{
   release();
   swap(*this, other);
   return *this;
}


bool PngTexture::load(const std::filesystem::path& texFile, bool flipVert)
{
   if (flipVert)
      stbi_set_flip_vertically_on_load(true);

   int numChannels = 0;
   m_data = stbi_load(texFile.string().c_str(), &m_width, &m_height, &numChannels, 0);
   if (!m_data)
      clear();

   return operator bool();
}


void PngTexture::release()
{
   if (m_data)
      stbi_image_free(m_data);
   clear();
}


void PngTexture::clear()
{
   m_width = 0;
   m_height = 0;
   m_data = nullptr;
}
