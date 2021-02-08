//
// Jan-2021, Michael Lindner
// MIT license
//
#pragma once
#include "coords.h"
#include "sge_sprite.h"
#include <memory>
#include <string>


///////////////////

struct PlaceSession
{
   std::string model;
   sge::Sprite indicator;
   MapCoord range = 1.f;
};
