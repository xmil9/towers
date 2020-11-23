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


bool populateMap(const std::string& mapStr, std::vector<Field>& map)
{
   if (mapStr.size() != map.size())
      return false;

   for (int i = 0; i < mapStr.size(); ++i)
      map[i] = makeField(mapStr[i]);

   return true;
}


bool populateTerrainData(glm::ivec2 mapSize, const std::string& map,
                                const std::vector<glm::ivec2>& starts, glm::ivec2 finish,
                                TerrainData& data)
{
   data.mapSize = mapSize;
   data.starts = starts;
   data.finish = finish;
   data.map.resize(mapSize.x * mapSize.y);
   return populateMap(map, data.map);
}


bool parseTerrainData(const json& jData, TerrainData& data)
{
   json jTerrain = jData["terrain"];

   glm::ivec2 mapSize;
   mapSize.x = jTerrain["rows"];
   mapSize.y = jTerrain["cols"];

   json jMap = jTerrain["map"];
   assert(jMap.size() == mapSize.x);
   std::string map;
   for (int i = 0; i < jMap.size(); ++i)
      map += jMap[i];

   std::vector<glm::ivec2> starts;
   json jStarts = jTerrain["starts"];
   for (int i = 0; i < jStarts.size(); ++i)
   {
      glm::ivec2 startPos;
      startPos.x = jStarts[i]["x"];
      startPos.y = jStarts[i]["y"];
      starts.push_back(startPos);
   }

   glm::ivec2 finish;
   finish.x = jTerrain["finish"]["x"];
   finish.y = jTerrain["finish"]["y"];

   return populateTerrainData(mapSize, map, starts, finish, data);
}

} // namespace


bool loadTerrainData(const std::filesystem::path& path, TerrainData& data)
{
   std::ifstream inStream{path.c_str()};
   json jData;
   inStream >> jData;

   return parseTerrainData(jData, data);
}
