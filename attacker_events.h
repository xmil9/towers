//
// Jan-2021, Michael Lindner
// MIT license
//
#pragma once
#include "observed.h"


///////////////////

// Notifications sent to observers.

constexpr char AttackerDestroyedMsg[] = "attacker_destroyed";
struct AttackerDestroyedMsgData : public ObservedMsgData
{
};
