//
// Jan-2021, Michael Lindner
// MIT license
//
#pragma once
#include "spiel/coords.h"
#include "spiel/sprite.h"
#include <memory>
#include <string>


///////////////////

struct PlaceSession
{
   std::string model;
   sp::Sprite indicator;
   sp::MapCoord range = 1.f;
};
