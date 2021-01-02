//
// Jan-2021, Michael Lindner
// MIT license
//
#pragma once
#include "sprite.h"
#include <memory>
#include <string>


///////////////////

struct PlaceSession
{
   std::string model;
   std::unique_ptr<Sprite> indicator;
};
