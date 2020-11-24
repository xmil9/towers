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
   std::vector<std::vector<glm::ivec2>> paths;

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
   for (int i = 0; i < jMap.size(); ++i)
      parsed.map += jMap[i];

   json jStarts = jTerrain["starts"];
   for (int i = 0; i < jStarts.size(); ++i)
      parsed.starts.push_back(parsePosition(jStarts[i]));

   parsed.finish = parsePosition(jTerrain["finish"]);

   json jPaths = jTerrain["paths"];
   for (int i = 0; i < jPaths.size(); ++i)
   {
      std::vector<FieldPos> parsedPath;

      json jPath = jPaths[i];
      for (int j = 0; j < jPath.size(); ++j)
         parsedPath.push_back(parsePosition(jPath[j]));

      parsed.paths.push_back(parsedPath);
   }

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
      return Field{true, false};
   case SiteSymbol:
      return Field{false, true};
   case OffSymbol:
      return Field{false, false};
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
   for (int i = 0; i < parsed.map.size(); ++i)
      terrain.map.push_back(makeField(parsed.map[i]));

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
