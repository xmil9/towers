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

std::vector<Rect> generateTerrainTurns(const std::vector<FieldPos>& turns,
                                       glm::vec2 fieldSize)
{
   std::vector<Rect> terrainTurns;
   terrainTurns.reserve(turns.size());
   std::transform(turns.begin(), turns.end(), std::back_inserter(terrainTurns),
                  [&fieldSize](FieldPos pos) {
                     Pos lt = glm::vec2(pos) * fieldSize;
                     return Rect{lt, lt + fieldSize};
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


std::optional<Rect> Path::nextTurn(const Pos& at) const
{
   auto turnIter = std::find_if(m_turns.cbegin(), m_turns.cend(),
                                 [&](const Rect& turn) { return turn.contains(at); });
   if (turnIter == m_turns.cend())
      throw std::runtime_error("Position not on path.");

   if (++turnIter == m_turns.cend())
      return std::nullopt;
   return *turnIter;
}
