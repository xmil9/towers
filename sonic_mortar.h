//
// Jan-2021, Michael Lindner
// MIT license
//
#pragma once
#include "defender_base.h"


///////////////////

class SonicMortar : public DefenderBase<SonicMortar>
{
   friend class DefenderBase<SonicMortar>;

 public:
   struct Attribs : public DefenderBase<SonicMortar>::Attribs
   {
      MapCoord collateralDamageRange = 0.f;
      int collateralDamage = 0;
   };

 public:
   SonicMortar(DefenderLook look, MapCoord size, MapPos center, const Attribs& attribs,
               const MapCoordSys* cs, std::vector<Attacker>* attackers);

 private:
   void shoot();
   bool isInCollateralRange(const Attacker& attacker) const;


 private:
   Attribs m_attribs;
};


inline SonicMortar::SonicMortar(DefenderLook look, MapCoord size, MapPos center,
                                const Attribs& attribs, const MapCoordSys* cs,
                                std::vector<Attacker>* attackers)
: DefenderBase<SonicMortar>{look, size, center, cs, attackers}, m_attribs{attribs}

{
}

inline void SonicMortar::shoot()
{
   if (!m_target || !(*m_target)->isAlive())
      return;

   // Hit the target with the primary damage.
   (*m_target)->hit(m_attribs.damage);

   // Hit other attackers in collateral range with the collateral damage.
   for (auto& attacker : *m_attackers)
   {
      const bool isPrimaryTarget = &attacker != *m_target;
      if (!isPrimaryTarget && attacker.isAlive() && isInCollateralRange(attacker))
         attacker.hit(m_attribs.collateralDamage);
   }
}


inline bool SonicMortar::isInCollateralRange(const Attacker& attacker) const
{
   if (!m_target)
      return false;

   const auto targetPos = (*m_target)->position();
   if (!targetPos)
      return false;

   const auto pos = attacker.position();
   if (!pos)
      return false;

   const MapCoord dist = glm::length(*targetPos - *pos);
   return sutil::lessEqual(dist, m_attribs.collateralDamageRange);
}
