//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "field.h"
#include "map_coords.h"
#include "path.h"
#include <cstddef>
#include <filesystem>
#include <optional>
#include <vector>


struct MapData
{
   // Number of rows and columns in the map.
   FieldDim mapSize;
   std::vector<Field> terrain;
   // Coordinates of start fields.
   std::vector<FieldPos> starts;
   // Coordinates of finish field.
   FieldPos finish;
   // Coordinates for paths through the map.
   std::vector<Path> paths;
};

std::optional<MapData> loadMapData(const std::filesystem::path& path);