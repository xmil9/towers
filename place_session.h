//
// Jan-2021, Michael Lindner
// MIT license
//
#pragma once
#include "sge_coords.h"
#include "sge_sprite.h"
#include <memory>
#include <string>


///////////////////

struct PlaceSession
{
   std::string model;
   sge::Sprite indicator;
   sge::MapCoord range = 1.f;
};
