//
// Nov-2020, Michael Lindner
// MIT license
//
#include "terrain_data.h"
#include "nlohmann/json.hpp"
#include <fstream>

using json = nlohmann::json;


namespace
{
///////////////////

json readFile(const std::filesystem::path& path)
{
   std::ifstream inStream{path.c_str()};
   json jData;
   inStream >> jData;
   return jData;
}


///////////////////

struct ParsedTerrainData
{
   glm::ivec2 mapSize;
   std::string map;
   std::vector<glm::ivec2> starts;
   glm::ivec2 finish;
   std::vector<std::vector<FieldPos>> paths;

   bool isValid() const;
};


bool ParsedTerrainData::isValid() const
{
   const int numFields = mapSize.x * mapSize.y;
   return numFields == map.size();
}


glm::ivec2 parsePosition(const json& jList)
{
   if (jList.size() != 2)
      throw std::runtime_error("Terrain import - Invalid position.");
   return {jList[0], jList[1]};
}


std::vector<FieldPos> parseJsonPath(const json& jPath)
{
   std::vector<FieldPos> parsedPath;
   parsedPath.reserve(jPath.size());
   std::transform(jPath.begin(), jPath.end(), std::back_inserter(parsedPath),
                  [](const json& jList) { return parsePosition(jList); });
   return parsedPath;
}


ParsedTerrainData parseJson(const json& jData)
{
   json jTerrain = jData["terrain"];

   ParsedTerrainData parsed;
   parsed.mapSize.x = jTerrain["rows"];
   parsed.mapSize.y = jTerrain["cols"];

   json jMap = jTerrain["map"];
   assert(jMap.size() == parsed.mapSize.x);
   if (jMap.size() != parsed.mapSize.x)
      throw std::runtime_error(
         "Terrain import - Number of map rows not matching map layout.");
   // Concat row strings.
   parsed.map = std::reduce(jMap.begin(), jMap.end());

   json jStarts = jTerrain["starts"];
   parsed.starts.reserve(jStarts.size());
   std::transform(jStarts.begin(), jStarts.end(), std::back_inserter(parsed.starts),
                  [](const json& jlist) { return parsePosition(jlist); });

   parsed.finish = parsePosition(jTerrain["finish"]);

   json jPaths = jTerrain["paths"];
   parsed.paths.reserve(jPaths.size());
   std::transform(jPaths.begin(), jPaths.end(), std::back_inserter(parsed.paths),
                  [](const json& jPath) { return parseJsonPath(jPath); });

   return parsed;
}


///////////////////

constexpr char PathSymbol = '.';
constexpr char SiteSymbol = '_';
constexpr char OffSymbol = 'x';


Field makeField(char symbol)
{
   switch (symbol)
   {
   case PathSymbol:
      return makePathField();
   case SiteSymbol:
      return makeSiteField();
   case OffSymbol:
      return makeOffField();
   default:
      throw std::runtime_error("Terrain import - Invalid field symbol.");
   }
}


TerrainData populateData(const ParsedTerrainData& parsed)
{
   TerrainData terrain;
   terrain.mapSize = parsed.mapSize;
   terrain.starts = parsed.starts;
   terrain.finish = parsed.finish;

   terrain.map.reserve(terrain.mapSize.x * terrain.mapSize.y);
   std::transform(parsed.map.cbegin(), parsed.map.cend(), std::back_inserter(terrain.map),
                  [](char fieldSymbol) { return makeField(fieldSymbol); });

   terrain.paths.reserve(parsed.paths.size());
   std::transform(parsed.paths.cbegin(), parsed.paths.cend(),
                  std::back_inserter(terrain.paths),
                  [](const std::vector<FieldPos>& turns) { return Path{turns}; });

   return terrain;
}

} // namespace


///////////////////

std::optional<TerrainData> loadTerrainData(const std::filesystem::path& path)
{
   try
   {
      return populateData(parseJson(readFile(path)));
   }
   catch (...)
   {
      return std::nullopt;
   }
}
