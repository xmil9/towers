//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "png_texture.h"
#include <filesystem>
#include <string>
#include <unordered_map>


class Textures
{
 public:
   bool load(const std::string& tag, const std::filesystem::path& texFile,
             bool flipVert = false);
   const PngTexture& operator[](const std::string& tag) const;
   void clear() { m_texs.clear(); }

 private:
   std::unordered_map<std::string, PngTexture> m_texs;
};
