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
   return m_mainPath;
}


std::filesystem::path Resources::shaderPath() const
{
   return m_mainPath;
}


std::filesystem::path Resources::mapPath() const
{
   return m_mainPath;
}
