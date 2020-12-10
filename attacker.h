//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "map_coord_sys.h"
#include "path.h"
#include "sprite.h"
#include "glm/glm.hpp"
#include <cstddef>

namespace gll
{
class Program;
}


///////////////////

class Attacker
{
 public:
   Attacker(Sprite sp, NormVec size, int hp, float speed, const OffsetPath& path,
            const MapCoordSys& cs);

   void render(const gll::Program& shaders);
   void update();

 private:
   void move();
   bool isAtLastPosition() const;
   void setPosition(std::optional<NormPos> center);
   void setSize(NormVec size);
   void calcRotation();
   // Returns normalized direction of movement.
   NormVec direction() const;
   NormVec normedDirection() const { return glm::normalize(direction()); }

 private:
   NormVec m_size;
   int m_hp = 0;
   float m_speed = .001f;
   std::optional<NormPos> m_center;
   std::optional<Path::Index> m_currTurn;
   OffsetPath m_path;
   const MapCoordSys& m_coordSys;
   Sprite m_sprite;
};
