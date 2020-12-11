//
// Dec-2020, Michael Lindner
// MIT license
//
#pragma once
#include "attacker.h"
#include "map_coord_sys.h"
#include "path.h"
#include "sprite.h"
#include "glm/glm.hpp"
#include <cstddef>
#include <optional>
#include <vector>

namespace gll
{
class Program;
}


///////////////////

class Defender
{
 public:
   Defender(Sprite sp, NormVec size, NormPos center, NormCoord range,
            const MapCoordSys& cs, std::vector<Attacker>& attackers);

   void render(const gll::Program& shaders);
   void update();

 private:
   void setPosition(NormPos center);
   void setSize(NormVec size);
   void findTarget();
   bool isInRange(const Attacker& attacker) const;
   void calcRotation();
   std::optional<NormVec> targetDirection() const;

 private:
   NormCoord m_range = 0.f;
   NormVec m_size;
   NormPos m_center;
   const MapCoordSys& m_coordSys;
   Sprite m_sprite;
   std::vector<Attacker>& m_attackers;
   std::optional<Attacker*> m_target;
};
