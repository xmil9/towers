//
// Feb-2021, Michael Lindner
// MIT license
//
#pragma once
#include "spiel/coords.h"
#include <string>
#include <vector>


///////////////////

struct AttackerSpec
{
   std::string model;
   sp::MapVec pathOffset{0.f, 0.f};
   // Small initial default delay to keep attackers from rendering before the attack has
   // started.
   int launchDelay = 1;
};


struct Level
{
   const std::string mapFileName;
   const sp::PixCoordi mapWidth;
   const sp::PixCoordi mapHeight;
   const std::string backgroundTex;
   std::vector<AttackerSpec> attackers;
};
