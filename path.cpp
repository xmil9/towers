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
/////////////////////
//
// int makeStepCoord(int delta)
//{
//   return delta > 0 ? 1 : (delta < 0) ? -1 : 0;
//}
//
//
// FieldPos makeStep(FieldPos delta)
//{
//   return {makeStepCoord(delta.x), makeStepCoord(delta.y)};
//}
//
//
/////////////////////
//
// std::vector<FieldPos> generateSegment(FieldPos from, FieldPos to)
//{
//   const FieldPos diff = to - from;
//   if (diff.x != 0 && diff.y != 0)
//      throw std::runtime_error("Path segment not vertical or horizontal.");
//   if (diff.x == 0 && diff.y == 0)
//      throw std::runtime_error("Path segment empty.");
//
//   const FieldPos step = makeStep(diff);
//   const int numSteps = std::max(std::abs(diff.x), std::abs(diff.y));
//
//   std::vector<FieldPos> segm;
//   segm.resize(numSteps);
//   *segm.begin() = from;
//   FieldPos curr = from;
//   std::generate(segm.begin() + 1, segm.end(), [&curr, &step]() { return curr += step;
//   });
//
//   return segm;
//}
//
//
//// Generates all path fields from given turns.
// std::vector<FieldPos> generateFields(const std::vector<FieldPos>& turns)
//{
//   if (turns.empty())
//      return {};
//   if (turns.size() == 1)
//      return turns;
//
//   std::vector<FieldPos> fields;
//   auto lastTurn = turns.cbegin();
//   for (auto nextTurn = lastTurn + 1; nextTurn != turns.cend(); ++nextTurn, ++lastTurn)
//   {
//      const std::vector<FieldPos> segm = generateSegment(*lastTurn, *nextTurn);
//      fields.insert(fields.end(), segm.begin(), segm.end());
//   }
//   fields.push_back(*turns.rbegin());
//
//   return fields;
//}


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


bool contains(Pos lt, Pos rb, Pos pos)
{
   return lt.x <= pos.x && pos.x < rb.x && lt.y <= pos.y && pos.y < rb.y;
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
   auto fieldIter = std::find_if(m_turns.begin(), m_turns.end(),
                                 [&](const Rect& turn) { return turn.contains(at); });
   if (fieldIter == m_turns.end())
      throw std::runtime_error("Position not on path.");

   if (fieldIter == m_turns.rbegin().base())
      return std::nullopt;
   return *++fieldIter;
}
