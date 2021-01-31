//
// Jan-2021, Michael Lindner
// MIT license
//
#pragma once
#include "attacker_base.h"
#include "attacker_models.h"


///////////////////

class AssaultTank : public AttackerBase<AssaultTank>
{
   friend class AttackerBase<AssaultTank>;

 public:
   using Attribs = AttackerBase<AssaultTank>::Attribs;

 public:
   AssaultTank(EntityId id, AttackerLook look, MapCoord size, const Attribs& attribs,
               const OffsetPath& path, const MapCoordSys* cs);

   static Attribs defaultAttributes();

 private:
   Attribs m_attribs;
};


inline AssaultTank::AssaultTank(EntityId id, AttackerLook look, MapCoord size,
                                const Attribs& attribs, const OffsetPath& path,
                                const MapCoordSys* cs)
: AttackerBase<AssaultTank>{id, look, size, attribs, path, cs}, m_attribs{attribs}

{
}

inline AssaultTank::Attribs AssaultTank::defaultAttributes()
{
   return Attribs{AatHp, AatSpeed, AatDelay, AatReward};
}
