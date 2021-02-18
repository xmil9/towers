//
// Jan-2021, Michael Lindner
// MIT license
//
#pragma once
#include "attacker_base.h"
#include "attacker_models.h"

class MapCoordSys;
class OffsetPath;


///////////////////

class MobileCannon : public AttackerBase<MobileCannon>
{
   friend class AttackerBase<MobileCannon>;

 public:
   using Attribs = AttackerBase<MobileCannon>::Attribs;

 public:
   MobileCannon(EntityId id, AttackerLook look, sp::MapDim size, const Attribs& attribs,
                const OffsetPath& path, const MapCoordSys* cs);

   static Attribs defaultAttributes();

 private:
   Attribs m_attribs;
};


inline MobileCannon::MobileCannon(EntityId id, AttackerLook look, sp::MapDim size,
                                  const Attribs& attribs, const OffsetPath& path,
                                  const MapCoordSys* cs)
: AttackerBase<MobileCannon>{id, look, size, attribs, path, cs}, m_attribs{attribs}

{
}

inline MobileCannon::Attribs MobileCannon::defaultAttributes()
{
   return Attribs{MhcHp, MhcSpeed, MhcDelay, MhcReward};
}
