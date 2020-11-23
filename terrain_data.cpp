//
// Nov-2020, Michael Lindner
// MIT license
//
#include "terrain_data.h"
#include "nlohmann/json.hpp"
#include <fstream>

using json = nlohmann::json;


/*

{
   "terrain": {
      "rows": <int>,
      "cols": <int>,
      "map": [
         ".", // path
         "*", // site
         "x"  // off
      ],
      "starts": [
         {"x": <int>, "y": <int>},
         {"x": <int>, "y": <int>}
      ],
      "finish": "x": <int>, "y": <int>}
   },
}

*/

static bool parseTerrainData(const json& jData, TerrainData& data)
{
   jData;
   data;
   return true;
}


bool loadTerrainData(const std::filesystem::path& path, TerrainData& data)
{
   std::ifstream inStream{path.c_str()};
   json jData;
   inStream >> jData;

   return parseTerrainData(jData, data);
}
