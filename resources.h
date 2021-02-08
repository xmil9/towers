//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "animations.h"
#include "textures.h"
#include <string>


class Resources
{
 public:
   Resources();

   std::filesystem::path texturePath() const;
   std::filesystem::path scenePath() const;
   std::filesystem::path uiPath() const;
   std::filesystem::path fontPath() const;
   std::filesystem::path shaderPath() const;
   std::filesystem::path mapPath() const;

   bool loadTexture(const std::string& tag, const std::filesystem::path& texFile,
                    bool flipVert = false);
   const gll::Texture2D& getTexture(const std::string& tag) const;
   sge::PixDim getTextureSize(const std::string& tag) const;
   void clearTextures() { m_texRepos.clear(); }

   void addAnimation(const std::string& tag, Animation&& a);
   const Animation& getAnimation(const std::string& tag) const;
   void clearAnimations() { m_animationRepos.clear(); }

 private:
   Textures m_texRepos;
   Animations m_animationRepos;
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

inline sge::PixDim Resources::getTextureSize(const std::string& tag) const
{
   return m_texRepos.size(tag);
}

inline void Resources::addAnimation(const std::string& tag, Animation&& a)
{
   m_animationRepos.add(tag, std::move(a));
}

inline const Animation& Resources::getAnimation(const std::string& tag) const
{
   return m_animationRepos[tag];
}
