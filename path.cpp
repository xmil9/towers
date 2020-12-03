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
                                       glm::vec2 fieldSize)
{
   std::vector<MapRect> terrainTurns;
   terrainTurns.reserve(turns.size());
   std::transform(turns.begin(), turns.end(), std::back_inserter(terrainTurns),
                  [&fieldSize](FieldPos pos) {
                     MapPos lt = glm::vec2(pos) * fieldSize;
                     return MapRect{lt, lt + fieldSize};
                  });
   return terrainTurns;
}

} // namespace


///////////////////

Path::Path(const std::vector<FieldPos>& turns, glm::vec2 fieldSize)
: m_turns{generateTerrainTurns(turns, fieldSize)}
{
   if (m_turns.empty())
      throw std::runtime_error("An empty path is illegal.");
}
