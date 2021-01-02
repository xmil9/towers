//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "attacker_look.h"
#include "map_coord_sys.h"
#include "path.h"
#include "glm/glm.hpp"
#include <optional>


///////////////////

class Attacker
{
 public:
   struct Attribs
   {
      int hp = 0;
      float speed = .1f;
      int launchDelay = 0;
   };

 public:
   Attacker(AttackerLook look, MapCoord size, const Attribs& attribs,
            const OffsetPath& path, const MapCoordSys* cs);

   void render(const gll::Program& shaders);
   void update();
   std::optional<MapPos> position() const { return m_center; }
   void hit(int damage);
   bool isAlive() const { return m_currAttribs.hp > 0; }
   bool hasStarted() const { return m_currAttribs.launchDelay == 0; }
   bool canBeRemoved() const;

 private:
   void move();
   bool isAtLastPosition() const;
   void setPosition(std::optional<MapPos> center);
   void setSize(MapVec size);
   void calcRotation();
   // Returns direction of movement.
   MapVec direction() const;
   MapVec normedDirection() const { return glm::normalize(direction()); }

 private:
   AttackerLook m_look;
   Attribs m_initialAttribs;
   Attribs m_currAttribs;
   std::optional<MapPos> m_center;
   std::optional<Path::Index> m_currTurn;
   OffsetPath m_path;
   const MapCoordSys* m_coordSys = nullptr;
};
