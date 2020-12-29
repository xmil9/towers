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
      NormCoord range = 0.f;
      int damage = 0;
    };

 public:
   Defender(DefenderLook look, NormVec size, NormPos center, const Attribs& attribs,
            const MapCoordSys* cs, std::vector<Attacker>& attackers);

   void render(const gll::Program& shaders);
   void update();

 private:
   void setPosition(NormPos center);
   void setSize(NormVec size);
   bool findTarget();
   bool isInRange(const Attacker& attacker) const;
   void calcRotation();
   std::optional<NormVec> targetDirection() const;
   void shoot();

 private:
   DefenderLook m_look;
   Attribs m_attribs;
   NormPos m_center;
   const MapCoordSys* m_coordSys;
   std::vector<Attacker>& m_attackers;
   std::optional<Attacker*> m_target;
};
