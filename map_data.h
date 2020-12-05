//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "coords.h"
#include "field.h"
#include "map_coord_sys.h"
#include "path.h"
#include <cstddef>
#include <filesystem>
#include <optional>
#include <vector>


struct MapData
{
   // Number of rows and columns in the map.
   IntDim mapSize;
   std::vector<Field> terrain;
   // Coordinates of start fields.
   std::vector<IntPos> starts;
   // Coordinates of finish field.
   IntPos finish;
   // Coordinates for paths through the map.
   std::vector<Path> paths;
};

std::optional<MapData> loadMapData(const std::filesystem::path& path);