//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "gll_texture.h"
#include <filesystem>
#include <string>
#include <unordered_map>


class Textures
{
 public:
   bool load(const std::string& tag, const std::filesystem::path& texFile,
             bool flipVert = false);
   const gll::Texture2D& operator[](const std::string& tag) const;
   void clear() { m_texs.clear(); }

 private:
   std::unordered_map<std::string, gll::Texture2D> m_texs;
};