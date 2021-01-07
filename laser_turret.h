//
// Jan-2021, Michael Lindner
// MIT license
//
#pragma once
#include "defender_base.h"


///////////////////

class LaserTurret : public DefenderBase<LaserTurret>
{
   friend class DefenderBase<LaserTurret>;

 public:
   using Attribs = DefenderBase<LaserTurret>::Attribs;

 public:
   LaserTurret(DefenderLook look, MapCoord size, MapPos center, const Attribs& attribs,
               const MapCoordSys* cs, std::vector<Attacker>* attackers);

 private:
   void shoot();

 private:
   Attribs m_attribs;
};


inline LaserTurret::LaserTurret(DefenderLook look, MapCoord size, MapPos center,
                                const Attribs& attribs, const MapCoordSys* cs,
                                std::vector<Attacker>* attackers)
: DefenderBase<LaserTurret>{look, size, center, cs, attackers}, m_attribs{attribs}

{
}

inline void LaserTurret::shoot()
{
   if (m_target || !(*m_target)->isAlive())
      (*m_target)->hit(m_attribs.damage);
}
