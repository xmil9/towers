//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "textures.h"


class Resources
{
 public:
   bool loadTexture(const std::string& tag, const std::filesystem::path& texFile,
                    bool flipVert = false);
   const PngTexture& getTexture(const std::string& tag) const;
   void clearTextures() { m_texRepos.clear(); }

 private:
   Textures m_texRepos;
};


inline bool Resources::loadTexture(const std::string& tag,
                                   const std::filesystem::path& texFile, bool flipVert)
{
   return m_texRepos.load(tag, texFile, flipVert);
}

inline const PngTexture& Resources::getTexture(const std::string& tag) const
{
   return m_texRepos[tag];
}
