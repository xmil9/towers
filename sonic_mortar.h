//
// Jan-2021, Michael Lindner
// MIT license
//
#pragma once
#include "defender_base.h"
#include "defender_models.h"


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
   SonicMortar(EntityId id, DefenderLook look, MapCoord size, MapPos center,
               const Attribs& attribs, const MapCoordSys* cs,
               std::unordered_map<EntityId, Attacker>* attackers);

   static Attribs defaultAttributes();

 private:
   void shoot();
   bool isInCollateralRange(const Attacker& attacker) const;


 private:
   Attribs m_attribs;
};


inline SonicMortar::SonicMortar(EntityId id, DefenderLook look, MapCoord size,
                                MapPos center, const Attribs& attribs,
                                const MapCoordSys* cs,
                                std::unordered_map<EntityId, Attacker>* attackers)
: DefenderBase<SonicMortar>{id, look, size, center, cs, attackers}, m_attribs{attribs}

{
}

inline SonicMortar::Attribs SonicMortar::defaultAttributes()
{
   return Attribs{SmRange, SmDamage, SmCost, SmCollateralRange, SmCollateralDamage};
}

inline void SonicMortar::shoot()
{
   if (!m_target || !target().isAlive())
      return;

   // Hit the target with the primary damage.
   target().hit(m_attribs.damage);

   // Hit other attackers in collateral range with the collateral damage.
   for (auto& entry : *m_attackers)
   {
      Attacker& attacker = entry.second;
      const bool isPrimaryTarget = attacker.id() == target().id();
      if (!isPrimaryTarget && attacker.isAlive() && isInCollateralRange(attacker))
         attacker.hit(m_attribs.collateralDamage);
   }
}


inline bool SonicMortar::isInCollateralRange(const Attacker& attacker) const
{
   if (!m_target)
      return false;

   const auto targetPos = target().position();
   if (!targetPos)
      return false;

   const auto pos = attacker.position();
   if (!pos)
      return false;

   const MapCoord dist = glm::length(*targetPos - *pos);
   return sutil::lessEqual(dist, m_attribs.collateralDamageRange);
}
