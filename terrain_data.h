//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "field.h"
#include "glm/vec2.hpp"
#include <cstddef>
#include <filesystem>
#include <optional>
#include <vector>


// Field position coordinates.
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
   // Coordinates for paths through the map.
   std::vector<std::vector<FieldPos>> paths;
};

std::optional<TerrainData> loadTerrainData(const std::filesystem::path& path);