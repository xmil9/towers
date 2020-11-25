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
   std::optional<FieldPos> m_pos;
};


inline Attacker::Attacker(Sprite sp, int hp, const Path& path)
   : m_sprite{std::move(sp)}, m_hp{hp}, m_path{path}, m_pos{path.first()}
{
   if (m_pos)
      m_sprite.setPosition(*m_pos * 30);
}

inline void Attacker::render(const gll::Program& shaders) const
{
   m_sprite.render(shaders);
}
