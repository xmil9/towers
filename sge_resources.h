//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "sge_animations.h"
#include "sge_textures.h"
#include <string>


namespace sge
{
///////////////////

class Resources
{
 public:
   bool loadTexture(const std::string& tag, const std::filesystem::path& texFile,
                    bool flipVert = false);
   const gll::Texture2D& getTexture(const std::string& tag) const;
   PixDim getTextureSize(const std::string& tag) const;
   void clearTextures() { m_texRepos.clear(); }

   void addAnimation(const std::string& tag, Animation&& a);
   const Animation& getAnimation(const std::string& tag) const;
   void clearAnimations() { m_animationRepos.clear(); }

 private:
   Textures m_texRepos;
   Animations m_animationRepos;
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

inline PixDim Resources::getTextureSize(const std::string& tag) const
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

} // namespace sge
