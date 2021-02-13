//
// Feb-2021, Michael Lindner
// MIT license
//
#pragma once
#include "essentutils/filesys.h"
#include <filesystem>


class Paths
{
 public:
   Paths();

   std::filesystem::path texturePath() const;
   std::filesystem::path scenePath() const;
   std::filesystem::path uiPath() const;
   std::filesystem::path fontPath() const;
   std::filesystem::path shaderPath() const;
   std::filesystem::path mapPath() const;

 private:
   std::filesystem::path m_mainPath;
};


inline Paths::Paths()
   : m_mainPath{esl::appDirectory()}
{
}

inline std::filesystem::path Paths::texturePath() const
{
   return m_mainPath / "resources" / "textures";
}

inline std::filesystem::path Paths::scenePath() const
{
   return texturePath() / "scenes" / "abstract";
}

inline std::filesystem::path Paths::uiPath() const
{
   return texturePath() / "ui";
}

inline std::filesystem::path Paths::fontPath() const
{
   return m_mainPath / "resources" / "fonts";
}

inline std::filesystem::path Paths::shaderPath() const
{
   return m_mainPath / "shaders";
}

inline std::filesystem::path Paths::mapPath() const
{
   return m_mainPath / "resources";
}
