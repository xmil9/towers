//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "coords.h"
#include "opengl_util/gll_texture.h"
#include <filesystem>
#include <string>
#include <unordered_map>


class Textures
{
 public:
   bool load(const std::string& tag, const std::filesystem::path& texFile,
             bool flipVert = false);
   const gll::Texture2D& operator[](const std::string& tag) const;
   PixDim size(const std::string& tag) const;
   void clear() { m_texs.clear(); }

 private:
    struct Entry
    {
       gll::Texture2D texture;
       PixDim size;
    };

 private:
   std::unordered_map<std::string, Entry> m_texs;
};
