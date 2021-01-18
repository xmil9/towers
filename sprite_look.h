//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "basic_types.h"
#include "glm/vec3.hpp"
#include <optional>
#include <string>


class SpriteLook
{
 public:
   SpriteLook() = default;
   explicit SpriteLook(std::string_view texTag);
   explicit SpriteLook(const Color& color);
   SpriteLook(std::string_view texTag, const Color& color);
   ~SpriteLook() = default;
   SpriteLook(const SpriteLook&) = default;
   SpriteLook(SpriteLook&&) = default;

   SpriteLook& operator=(const SpriteLook&) = default;
   SpriteLook& operator=(SpriteLook&&) = default;

   bool hasTexture() const { return m_textureTag.has_value(); }
   std::string texture() const;
   bool hasColor() const { return m_color != White; }
   Color color() const { return m_color; }

 private:
   std::optional<std::string> m_textureTag;
   Color m_color;
};


inline SpriteLook::SpriteLook(std::string_view texTag) : SpriteLook{texTag, White}
{
}

inline SpriteLook::SpriteLook(const Color& color) : m_color{color}
{
}

inline SpriteLook::SpriteLook(std::string_view texTag, const Color& color)
: m_textureTag{texTag}, m_color{color}
{
}

inline std::string SpriteLook::texture() const
{
   if (hasTexture())
      return *m_textureTag;
   return "";
}
