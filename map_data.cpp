//
// Nov-2020, Michael Lindner
// MIT license
//
#include "map_data.h"
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

struct ParsedMapData
{
   glm::ivec2 mapSize;
   std::string terrain;
   std::vector<glm::ivec2> starts;
   glm::ivec2 finish;
   std::vector<std::vector<FieldPos>> paths;

   bool isValid() const;
};


bool ParsedMapData::isValid() const
{
   const int numFields = mapSize.x * mapSize.y;
   return numFields == terrain.size();
}


glm::ivec2 parsePosition(const json& jList)
{
   if (jList.size() != 2)
      throw std::runtime_error("Map import - Invalid position.");
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


ParsedMapData parseJson(const json& jData)
{
   json jMap = jData["map"];

   ParsedMapData parsed;
   parsed.mapSize.x = jMap["cols"];
   parsed.mapSize.y = jMap["rows"];

   json jTerrain = jMap["terrain"];
   assert(jTerrain.size() == parsed.mapSize.y);
   if (jTerrain.size() != parsed.mapSize.y)
      throw std::runtime_error(
         "Map import - Number of map rows not matching map layout.");
   // Concat row strings.
   for (const auto& row : jTerrain)
      parsed.terrain += row;

   json jStarts = jMap["starts"];
   parsed.starts.reserve(jStarts.size());
   std::transform(jStarts.begin(), jStarts.end(), std::back_inserter(parsed.starts),
                  [](const json& jlist) { return parsePosition(jlist); });

   parsed.finish = parsePosition(jMap["finish"]);

   json jPaths = jMap["paths"];
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
      throw std::runtime_error("Map import - Invalid field symbol.");
   }
}


MapData populateData(const ParsedMapData& parsed)
{
   MapData map;
   map.mapSize = parsed.mapSize;
   map.starts = parsed.starts;
   map.finish = parsed.finish;

   map.terrain.reserve(map.mapSize.x * map.mapSize.y);
   std::transform(parsed.terrain.cbegin(), parsed.terrain.cend(),
                  std::back_inserter(map.terrain),
                  [](char fieldSymbol) { return makeField(fieldSymbol); });

   const glm::vec2 fieldSize{1.f / static_cast<float>(map.mapSize.x),
                             1.f / static_cast<float>(map.mapSize.y)};

   map.paths.reserve(parsed.paths.size());
   std::transform(parsed.paths.cbegin(), parsed.paths.cend(),
                  std::back_inserter(map.paths),
                  [&fieldSize](const std::vector<FieldPos>& turns) {
                     return Path{turns, fieldSize};
                  });

   return map;
}

} // namespace


///////////////////

std::optional<MapData> loadMapData(const std::filesystem::path& path)
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