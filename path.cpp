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

std::vector<NormRect> generateTerrainTurns(const std::vector<IntPos>& turns,
                                           NormDim fieldSize)
{
   std::vector<NormRect> terrainTurns;
   terrainTurns.reserve(turns.size());
   std::transform(turns.begin(), turns.end(), std::back_inserter(terrainTurns),
                  [&fieldSize](IntPos pos) {
                     const NormPos lt = NormPos(pos) * fieldSize;
                     return NormRect{lt, lt + fieldSize};
                  });
   return terrainTurns;
}

} // namespace


///////////////////

Path::Path(const std::vector<IntPos>& turns, NormDim fieldSize)
: m_turns{generateTerrainTurns(turns, fieldSize)}
{
   if (m_turns.empty())
      throw std::runtime_error("An empty path is illegal.");
}
