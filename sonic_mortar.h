//
// Jan-2021, Michael Lindner
// MIT license
//
#pragma once
#include "defender_base.h"


///////////////////

class SonicMortar : public DefenderBase<SonicMortar>
{
 public:
   using Attribs = DefenderBase<SonicMortar>::Attribs;

 public:
   SonicMortar(DefenderLook look, MapCoord size, MapPos center, const Attribs& attribs,
               const MapCoordSys* cs, std::vector<Attacker>* attackers);

   void shoot();
};


inline SonicMortar::SonicMortar(DefenderLook look, MapCoord size, MapPos center,
                                const Attribs& attribs, const MapCoordSys* cs,
                                std::vector<Attacker>* attackers)
: DefenderBase<SonicMortar>{look, size, center, attribs, cs, attackers}

{
}

inline void SonicMortar::shoot()
{
   if (m_target)
      (*m_target)->hit(m_attribs.damage);
}
