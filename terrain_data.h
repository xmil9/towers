//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "field.h"
#include "glm/vec2.hpp"
#include <cstddef>
#include <filesystem>
#include <vector>


using FieldPos = glm::ivec2;

struct TerrainData
{
   // Rows and columns in the map.
   glm::ivec2 mapSize;
   std::vector<Field> map;
   // Coordinates of start fields.
   std::vector<FieldPos> starts;
   // Coordinates of finish field.
   FieldPos finish;
};


bool loadTerrainData(const std::filesystem::path& path, TerrainData& data);