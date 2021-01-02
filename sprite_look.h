//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "glm/vec3.hpp"
#include <optional>
#include <string>


class SpriteLook
{
 public:
   SpriteLook() = default;
   explicit SpriteLook(std::string_view texTag);
   explicit SpriteLook(const glm::vec3& color);
   SpriteLook(std::string_view texTag, const glm::vec3& color);
   ~SpriteLook() = default;
   SpriteLook(const SpriteLook&) = default;
   SpriteLook(SpriteLook&&) = default;

   SpriteLook& operator=(const SpriteLook&) = default;
   SpriteLook& operator=(SpriteLook&&) = default;

   bool hasTexture() const { return m_textureTag.has_value(); }
   std::string texture() const;
   bool hasColor() const { return m_color != White; }
   glm::vec3 color() const { return m_color; }

 private:
   static constexpr glm::vec3 White{1.f, 1.f, 1.f};
   std::optional<std::string> m_textureTag;
   glm::vec3 m_color;
};


inline SpriteLook::SpriteLook(std::string_view texTag) : SpriteLook{texTag, White}
{
}

inline SpriteLook::SpriteLook(const glm::vec3& color) : m_color{color}
{
}

inline SpriteLook::SpriteLook(std::string_view texTag, const glm::vec3& color)
: m_textureTag{texTag}, m_color{color}
{
}

inline std::string SpriteLook::texture() const
{
   if (hasTexture())
      return *m_textureTag;
   return "";
}
