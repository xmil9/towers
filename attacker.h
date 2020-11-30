//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "coord_sys.h"
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
   Attacker(Sprite sp, int hp, const Path& path, const CoordSys& cs);

   void render(const gll::Program& shaders) const;
   void update();

 private:
   float m_speed = .001f;
   int m_hp = 0;
   std::optional<Pos> m_pos;
   Sprite m_sprite;
   const Path& m_path;
   const CoordSys& m_coordSys;
};


inline Attacker::Attacker(Sprite sp, int hp, const Path& path, const CoordSys& cs)
: m_sprite{std::move(sp)}, m_hp{hp}, m_pos{path.start().lt}, m_path{path}, m_coordSys{cs}
{
   if (m_pos)
      m_sprite.setPosition(m_coordSys.toRenderCoords(*m_pos));
}

inline void Attacker::render(const gll::Program& shaders) const
{
   m_sprite.render(shaders);
}
