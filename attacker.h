//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "path.h"
#include "sprite.h"
#include <cstddef>

namespace gll
{
class Program;
}


///////////////////

class Attacker
{
public:
   Attacker(Sprite sp, int hp, const Path& path);

   void render(const gll::Program& shaders) const;
   void advance();

private:
   float m_speed = 0.f;
   int m_hp = 0;
   Sprite m_sprite;
   const Path& m_path;
   std::optional<Pos> m_pos;
};


inline Attacker::Attacker(Sprite sp, int hp, const Path& path)
   : m_sprite{std::move(sp)}, m_hp{hp}, m_path{path}, m_pos{path.start().lt}
{
   if (m_pos)
   {
      const glm::vec2 pixelPos{m_pos->x * 300, m_pos->y * 200};
      m_sprite.setPosition(pixelPos);
   }
}

inline void Attacker::render(const gll::Program& shaders) const
{
   m_sprite.render(shaders);
}
