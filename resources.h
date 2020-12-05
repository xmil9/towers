//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "textures.h"
#include <string>


class Resources
{
 public:
   Resources();

   std::filesystem::path texturePath() const;
   std::filesystem::path shaderPath() const;
   std::filesystem::path mapPath() const;

   bool loadTexture(const std::string& tag, const std::filesystem::path& texFile,
                    bool flipVert = false);
   const gll::Texture2D& getTexture(const std::string& tag) const;
   RenderDim getTextureSize(const std::string& tag) const;
   void clearTextures() { m_texRepos.clear(); }

 private:
   Textures m_texRepos;
   std::filesystem::path m_mainPath;
};


inline bool Resources::loadTexture(const std::string& tag,
                                   const std::filesystem::path& texFile, bool flipVert)
{
   return m_texRepos.load(tag, texFile, flipVert);
}

inline const gll::Texture2D& Resources::getTexture(const std::string& tag) const
{
   return m_texRepos[tag];
}

inline RenderDim Resources::getTextureSize(const std::string& tag) const
{
   return m_texRepos.size(tag);
}
