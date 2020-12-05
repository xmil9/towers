//
// Nov-2020, Michael Lindner
// MIT license
//
#include "textures.h"
#include "png_texture.h"


static gll::Texture2D NullTexture;


bool Textures::load(const std::string& tag, const std::filesystem::path& texFile,
                    bool flipVert)
{
   PngTexture tex{texFile, flipVert};
   if (!tex)
      return false;

   gll::Texture2D glTex;
   if (!glTex.create())
      return false;

   glTex.bind();
   glTex.setWrapPolicy(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
   glTex.setScaleFiltering(GL_NEAREST, GL_NEAREST);
   glTex.setData(0, tex.internalFormat(), tex.width(), tex.height(), tex.format(),
                 tex.pixelType(), tex.data());
   glTex.generateMipmap();

   const auto [pos, ok] = m_texs.insert_or_assign(
      tag, Entry{std::move(glTex), RenderDim(tex.width(), tex.height())});
   return ok;
}


const gll::Texture2D& Textures::operator[](const std::string& tag) const
{
   const auto pos = m_texs.find(tag);
   return (pos != m_texs.end()) ? pos->second.texture : NullTexture;
}


RenderDim Textures::size(const std::string& tag) const
{
   const auto pos = m_texs.find(tag);
   return (pos != m_texs.end()) ? pos->second.size : RenderDim{};
}
