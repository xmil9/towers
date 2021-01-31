//
// Jan-2021, Michael Lindner
// MIT license
//
#pragma once
#include "attacker.h"
#include "essentutils/fputil.h"


///////////////////

template <typename SpecificDefender> class FirstMatchTargetScan
{
 public:
   explicit FirstMatchTargetScan(AttackerMap* attackers);

   std::optional<EntityId> scan(SpecificDefender& defender);

 private:
   AttackerMap* m_attackers;
};


template <typename SpecificDefender>
FirstMatchTargetScan<SpecificDefender>::FirstMatchTargetScan(AttackerMap* attackers)
: m_attackers{attackers}
{
   assert(m_attackers);
}

template <typename SpecificDefender>
std::optional<EntityId>
FirstMatchTargetScan<SpecificDefender>::scan(SpecificDefender& defender)
{
   const auto iter = std::find_if(
      m_attackers->begin(), m_attackers->end(), [&defender](const auto& entry) {
         const Attacker& attacker = entry.second;
         return attacker.isAlive() && defender.isInRange(attacker);
      });
   return iter != m_attackers->end() ? std::make_optional(iter->second.id())
                                     : std::nullopt;
}


///////////////////

template <typename SpecificDefender> class ClosestTargetScan
{
 public:
   explicit ClosestTargetScan(AttackerMap* attackers);

   std::optional<EntityId> scan(SpecificDefender& defender);

 private:
   AttackerMap* m_attackers;
};


template <typename SpecificDefender>
ClosestTargetScan<SpecificDefender>::ClosestTargetScan(AttackerMap* attackers)
: m_attackers{attackers}
{
   assert(m_attackers);
}

template <typename SpecificDefender>
std::optional<EntityId>
ClosestTargetScan<SpecificDefender>::scan(SpecificDefender& defender)
{
   std::optional<EntityId> targetId;
   MapCoord minDist = std::numeric_limits<MapCoord>::max();

   for (const auto& entry : *m_attackers)
   {
      const Attacker& attacker = entry.second;
      if (attacker.isAlive() && attacker.position())
      {
         const MapCoord dist = glm::length(defender.center() - *attacker.position());
         if (defender.isInRange(dist) && sutil::lessEqual(dist, minDist))
         {
            targetId = attacker.id();
            minDist = dist;
         }
      }
   }

   return targetId;
}
