//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "field.h"
#include "path.h"
#include "terrain_types.h"
#include <cstddef>
#include <filesystem>
#include <optional>
#include <vector>


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
   std::vector<Path> paths;
};

std::optional<TerrainData> loadTerrainData(const std::filesystem::path& path);