//
// Nov-2020, Michael Lindner
// MIT license
//
#include "textures.h"


static PngTexture NullTexture;


bool Textures::load(const std::string& tag, const std::filesystem::path& texFile,
                    bool flipVert)
{
   PngTexture tex{texFile, flipVert};
   if (!tex)
      return false;

   const auto [pos, ok] = m_texs.insert_or_assign(tag, std::move(tex));
   return ok;
}


const PngTexture& Textures::operator[](const std::string& tag) const
{
   const auto pos = m_texs.find(tag);
   return (pos != m_texs.end()) ? pos->second : NullTexture;
}
