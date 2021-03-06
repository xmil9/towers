//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "field.h"
#include "path.h"
#include "spiel/coords.h"
#include <cstddef>
#include <filesystem>
#include <optional>
#include <vector>


struct MapData
{
   // Number of rows and columns in the map.
   sp::IntDim mapSize{0, 0};
   std::vector<Field> terrain;
   // Coordinates of start fields.
   std::vector<sp::IntPos> starts;
   // Coordinates of finish field.
   sp::IntPos finish{0, 0};
   // Coordinates for paths through the map.
   std::vector<Path> paths;
};

std::optional<MapData> loadMapData(const std::filesystem::path& path);