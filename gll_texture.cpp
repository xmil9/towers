//
// Oct-2020, Michael Lindner
// MIT license
//
#include "gll_texture.h"
#include "stb/stb_image.h"


namespace gll
{
///////////////////

void Texture2D::bind() const
{
   if (hasId())
      glBindTexture(GL_TEXTURE_2D, id());
}


void Texture2D::unbind()
{
   glBindTexture(GL_TEXTURE_2D, 0);
}


void Texture2D::setData(GLint level, GLint internalFormat, GLsizei width, GLsizei height,
                        GLenum format, GLenum type, const void* data)
{
   if (hasId())
      glTexImage2D(GL_TEXTURE_2D, level, internalFormat, width, height, 0, format, type,
                   data);
}


bool Texture2D::loadData(const std::filesystem::path& texFile, bool flipVert, GLint level,
                         GLint internalFormat, GLenum format, GLenum type)
{
   if (!hasId())
      return false;

   if (flipVert)
      stbi_set_flip_vertically_on_load(true);

   int width = 0;
   int height = 0;
   int numChannels = 0;
   unsigned char* data =
      stbi_load(texFile.string().c_str(), &width, &height, &numChannels, 0);
   if (!data)
      return false;

   setData(level, internalFormat, width, height, format, type, data);

   stbi_image_free(data);
   return true;
}


void Texture2D::generateMipmap()
{
   if (hasId())
      glGenerateMipmap(GL_TEXTURE_2D);
}


void Texture2D::setWrapPolicy(GLint xPolicy, GLint yPolicy)
{
   if (!hasId())
      return;

   if (xPolicy != -1)
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, xPolicy);
   if (yPolicy != -1)
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, yPolicy);
}


void Texture2D::setScaleFiltering(GLint minifyFilter, GLint magnifyFilter)
{
   if (!hasId())
      return;

   if (minifyFilter != -1)
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minifyFilter);
   if (magnifyFilter != -1)
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magnifyFilter);
}


ObjId Texture2D::create_()
{
   ObjId id;
   glGenTextures(1, &id);
   return id;
}


void Texture2D::destroy_(ObjId id)
{
   glDeleteTextures(1, &id);
}

} // namespace gll
