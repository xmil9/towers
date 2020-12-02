//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "map_coords.h"
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
   Attacker(Sprite sp, int hp, const Path& path, const MapCoordSys& cs);

   void render(const gll::Program& shaders) const { m_sprite.render(shaders); }
   void update();

 private:
   void move();
   bool isAtLastPosition() const;
   void setPosition(std::optional<Pos> pos);

 private:
   float m_speed = .001f;
   int m_hp = 0;
   std::optional<Pos> m_pos;
   std::optional<Path::Index> m_currTurn;
   const Path& m_path;
   const MapCoordSys& m_coordSys;
   Sprite m_sprite;
};
