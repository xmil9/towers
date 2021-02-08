//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "coords.h"
#include "field.h"
#include "path.h"
#include <cstddef>
#include <filesystem>
#include <optional>
#include <vector>


struct MapData
{
   // Number of rows and columns in the map.
   sge::IntDim mapSize{0, 0};
   std::vector<Field> terrain;
   // Coordinates of start fields.
   std::vector<sge::IntPos> starts;
   // Coordinates of finish field.
   sge::IntPos finish{0, 0};
   // Coordinates for paths through the map.
   std::vector<Path> paths;
};

std::optional<MapData> loadMapData(const std::filesystem::path& path);