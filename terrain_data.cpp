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

std::optional<json> readTerrainFile(const std::filesystem::path& path)
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

   bool isValid() const;
};


bool ParsedTerrainData::isValid() const
{
   const int numFields = mapSize.x * mapSize.y;
   return numFields == map.size();
}


std::optional<ParsedTerrainData> parseTerrainJson(const std::optional<json>& jData)
{
   if (!jData)
      return std::nullopt;

   json jTerrain = (*jData)["terrain"];

   ParsedTerrainData parsed;
   parsed.mapSize.x = jTerrain["rows"];
   parsed.mapSize.y = jTerrain["cols"];

   json jMap = jTerrain["map"];
   assert(jMap.size() == parsed.mapSize.x);
   for (int i = 0; i < jMap.size(); ++i)
      parsed.map += jMap[i];

   json jStarts = jTerrain["starts"];
   for (int i = 0; i < jStarts.size(); ++i)
   {
      glm::ivec2 startPos;
      startPos.x = jStarts[i]["x"];
      startPos.y = jStarts[i]["y"];
      parsed.starts.push_back(startPos);
   }

   parsed.finish.x = jTerrain["finish"]["x"];
   parsed.finish.y = jTerrain["finish"]["y"];

   return parsed.isValid() ? std::make_optional(parsed) : std::nullopt;
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
   default:
      assert(false && "Unknown terrain symbol.");
      [[fallthrough]];
   case OffSymbol:
      return Field{false, false};
   }
}


std::optional<TerrainData>
populateTerrainData(const std::optional<ParsedTerrainData>& parsed)
{
   if (!parsed)
      return std::nullopt;
   const ParsedTerrainData& parsedData = *parsed;

   TerrainData terrain;
   terrain.mapSize = parsedData.mapSize;
   terrain.starts = parsedData.starts;
   terrain.finish = parsedData.finish;

   terrain.map.reserve(terrain.mapSize.x * terrain.mapSize.y);
   for (int i = 0; i < parsedData.map.size(); ++i)
      terrain.map.push_back(makeField(parsedData.map[i]));

   return terrain;
}

} // namespace


///////////////////

std::optional<TerrainData> loadTerrainData(const std::filesystem::path& path)
{
   return populateTerrainData(parseTerrainJson(readTerrainFile(path)));
}
