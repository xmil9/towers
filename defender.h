//
// Dec-2020, Michael Lindner
// MIT license
//
#pragma once
#include "animation.h"
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
   Defender(Sprite sp, NormVec size, NormPos center, NormCoord range, int damage,
            Animation firing, const MapCoordSys* cs,
            std::vector<Attacker>& attackers);

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
   NormCoord m_range = 0.f;
   int m_damage = 0;
   NormVec m_size;
   NormPos m_center;
   const MapCoordSys* m_coordSys;
   Sprite m_sprite;
   Animation m_firing;
   std::vector<Attacker>& m_attackers;
   std::optional<Attacker*> m_target;
};
