//
// Dec-2020, Michael Lindner
// MIT license
//
#pragma once
#include "attacker.h"
#include "defender_look.h"
#include "map_coord_sys.h"
#include "path.h"
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
   struct Attribs
   {
      MapCoord range = 0.f;
      int damage = 0;
   };

 public:
   Defender(DefenderLook look, MapCoord size, MapPos center, const Attribs& attribs,
            const MapCoordSys* cs, std::vector<Attacker>* attackers);

   void render(const gll::Program& shaders);
   void update();

 private:
   void setPosition(MapPos center);
   void setSize(MapVec size);
   bool findTarget();
   bool isInRange(const Attacker& attacker) const;
   void calcRotation();
   std::optional<MapVec> targetDirection() const;
   void shoot();

 private:
   DefenderLook m_look;
   Attribs m_attribs;
   MapPos m_center;
   const MapCoordSys* m_coordSys;
   std::vector<Attacker>* m_attackers = nullptr;
   std::optional<Attacker*> m_target;
};
