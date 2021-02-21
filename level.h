//
// Feb-2021, Michael Lindner
// MIT license
//
#pragma once
#include "spiel/coords.h"
#include <string>
#include <vector>


///////////////////

// Small initial default delay to keep attackers from rendering before the attack has
// started.
constexpr int DefaultDelay = 1;

struct AttackerSpec
{
   std::string model;
   sp::MapVec pathOffset{0.f, 0.f};
   int launchDelay = DefaultDelay;
};


struct Level
{
   const int credits = 0;
   const std::string mapFileName;
   const std::string backgroundTex;
   const std::vector<AttackerSpec> attackers;
};
