//
// Jan-2021, Michael Lindner
// MIT license
//
#pragma once
#include "observed.h"


///////////////////

// Notifications sent to observers.

constexpr char AttackerDestroyedEvent[] = "attacker_destroyed";
struct AttackerDestroyedData : public ObservedEventData
{
};
