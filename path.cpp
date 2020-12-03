//
// Nov-2020, Michael Lindner
// MIT license
//
#include "path.h"
#include <algorithm>
#include <iterator>
#include <stdexcept>


namespace
{
///////////////////

std::vector<MapRect> generateTerrainTurns(const std::vector<FieldPos>& turns,
                                          MapDim fieldSize)
{
   std::vector<MapRect> terrainTurns;
   terrainTurns.reserve(turns.size());
   std::transform(turns.begin(), turns.end(), std::back_inserter(terrainTurns),
                  [&fieldSize](FieldPos pos) {
                     const MapPos lt = MapPos(pos) * fieldSize;
                     return MapRect{lt, lt + fieldSize};
                  });
   return terrainTurns;
}

} // namespace


///////////////////

Path::Path(const std::vector<FieldPos>& turns, MapDim fieldSize)
: m_turns{generateTerrainTurns(turns, fieldSize)}
{
   if (m_turns.empty())
      throw std::runtime_error("An empty path is illegal.");
}
