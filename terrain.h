//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "terrain_data.h"


class Terrain
{
public:

private:
   TerrainData m_rep;
   // Size of each field in pixels.
   glm::ivec2 fieldSize;
};
