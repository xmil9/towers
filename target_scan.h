//
// Jan-2021, Michael Lindner
// MIT license
//
#pragma once
#include "attacker.h"
#include "essentutils/fputil.h"


///////////////////

inline bool isInRange(const Attacker& attacker, MapPos from, MapCoord range)
{
   const auto pos = attacker.position();
   if (!pos)
      return false;

   const MapCoord dist = glm::length(from - *pos);
   return sutil::lessEqual(dist, range);
}


///////////////////

template <typename SpecificDefender> class FirstMatchTargetScan
{
 public:
   explicit FirstMatchTargetScan(AttackerMap* attackers);

   std::optional<EntityId> scan(MapPos center, MapCoord range);

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
std::optional<EntityId> FirstMatchTargetScan<SpecificDefender>::scan(MapPos from,
                                                                     MapCoord range)
{
   const auto iter = std::find_if(
      m_attackers->begin(), m_attackers->end(), [&from, &range](const auto& entry) {
         const Attacker& attacker = entry.second;
         return attacker.isAlive() && isInRange(attacker, from, range);
      });
   return iter != m_attackers->end() ? std::make_optional(iter->second.id())
                                     : std::nullopt;
}
