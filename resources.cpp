//
// Nov-2020, Michael Lindner
// MIT license
//
#include "resources.h"
#include "essentutils/filesys.h"


Resources::Resources()
   : m_mainPath{sutil::appDirectory()}
{
}


std::filesystem::path Resources::texturePath() const
{
   return m_mainPath / "resources" / "textures";
}


std::filesystem::path Resources::scenePath() const
{
   return texturePath() / "scenes" / "abstract";
}


std::filesystem::path Resources::uiPath() const
{
   return texturePath() / "ui";
}


std::filesystem::path Resources::fontPath() const
{
   return m_mainPath / "resources" / "fonts";
}


std::filesystem::path Resources::shaderPath() const
{
   return m_mainPath / "shaders";
}


std::filesystem::path Resources::mapPath() const
{
   return m_mainPath / "resources";
}
