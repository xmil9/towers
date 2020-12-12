//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "animation.h"
#include "map_coord_sys.h"
#include "path.h"
#include "sprite.h"
#include "glm/glm.hpp"
#include <cstddef>
#include <memory>
#include <vector>

namespace gll
{
class Program;
}


///////////////////

class Attacker
{
 public:
   Attacker(Sprite sp, NormVec size, int hp, float speed, const OffsetPath& path,
            Animation explosion,
            std::vector<Animation>* activeExplosions,
            const MapCoordSys* cs);

   void render(const gll::Program& shaders);
   void update();
   std::optional<NormPos> position() const { return m_center; }
   void damage(int amount);
   bool isAlive() const { return m_hp > 0; }

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
   const MapCoordSys* m_coordSys = nullptr;
   Sprite m_sprite;
   Animation m_explosion;
   std::vector<Animation>* m_activeExplosions;
};
