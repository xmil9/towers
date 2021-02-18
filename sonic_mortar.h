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
      sp::MapCoord collateralDamageRange = 0.f;
      int collateralDamage = 0;
   };

 public:
   SonicMortar(EntityId id, DefenderLook look, sp::MapDim size, sp::MapPos center,
               const Attribs& attribs, const MapCoordSys* cs, AttackerMap* attackers);

   static Attribs defaultAttributes();

 private:
   void shoot();
   bool isInCollateralRange(const Attacker& attacker) const;


 private:
   Attribs m_attribs;
};


inline SonicMortar::SonicMortar(EntityId id, DefenderLook look, sp::MapDim size,
                                sp::MapPos center, const Attribs& attribs,
                                const MapCoordSys* cs, AttackerMap* attackers)
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
   const auto targetId = target().id();
   target().hit(m_attribs.damage);
   // Caution - After hitting the target it might be destroyed, so accessing it in the
   //           rest of the function has to be protected again with another check!

   // Hit other attackers in collateral range with the collateral damage.
   for (auto& entry : *m_attackers)
   {
      Attacker& attacker = entry.second;
      const bool isPrimaryTarget = attacker.id() == targetId;
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

   const sp::MapCoord dist = glm::length(*targetPos - *pos);
   return esl::lessEqual(dist, m_attribs.collateralDamageRange);
}
