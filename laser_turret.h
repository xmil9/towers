//
// Jan-2021, Michael Lindner
// MIT license
//
#pragma once
#include "defender_base.h"
#include "defender_models.h"


///////////////////

class LaserTurret : public DefenderBase<LaserTurret>
{
   friend class DefenderBase<LaserTurret>;

 public:
   using Attribs = DefenderBase<LaserTurret>::Attribs;

 public:
   LaserTurret(EntityId id, DefenderLook look, MapCoord size, MapPos center,
               const Attribs& attribs, const MapCoordSys* cs,
               std::unordered_map<EntityId, Attacker>* attackers);

   static Attribs defaultAttributes();

 private:
   void shoot();

 private:
   Attribs m_attribs;
};


inline LaserTurret::LaserTurret(EntityId id, DefenderLook look, MapCoord size,
                                MapPos center, const Attribs& attribs,
                                const MapCoordSys* cs,
                                std::unordered_map<EntityId, Attacker>* attackers)
: DefenderBase<LaserTurret>{id, look, size, center, cs, attackers}, m_attribs{attribs}

{
}

inline void LaserTurret::shoot()
{
   if (m_target || !target().isAlive())
      target().hit(m_attribs.damage);
}

inline LaserTurret::Attribs LaserTurret::defaultAttributes()
{
   return Attribs{LtRange, LtDamage, LtCost};
}
