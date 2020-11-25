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

int makeStepCoord(int delta)
{
   return delta > 0 ? 1 : (delta < 0) ? -1 : 0;
}


FieldPos makeStep(FieldPos delta)
{
   return {makeStepCoord(delta.x), makeStepCoord(delta.y)};
}


///////////////////

std::vector<FieldPos> generateSegment(FieldPos from, FieldPos to)
{
   const FieldPos diff = to - from;
   if (diff.x != 0 && diff.y != 0)
      throw std::runtime_error("Path segment not vertical or horizontal.");
   if (diff.x == 0 && diff.y == 0)
      throw std::runtime_error("Path segment empty.");

   const FieldPos step = makeStep(diff);
   const int numSteps = std::max(std::abs(diff.x), std::abs(diff.y));

   std::vector<FieldPos> segm;
   segm.resize(numSteps);
   *segm.begin() = from;
   FieldPos curr = from;
   std::generate(segm.begin() + 1, segm.end(), [&curr, &step]() { return curr += step; });

   return segm;
}


// Generates all path fields from given turns.
std::vector<FieldPos> generateFields(const std::vector<FieldPos>& turns)
{
   if (turns.empty())
      return {};
   if (turns.size() == 1)
      return turns;

   std::vector<FieldPos> fields;
   auto lastTurn = turns.cbegin();
   for (auto nextTurn = lastTurn + 1; nextTurn != turns.cend(); ++nextTurn, ++lastTurn)
   {
      const std::vector<FieldPos> segm = generateSegment(*lastTurn, *nextTurn);
      fields.insert(fields.end(), segm.begin(), segm.end());
   }
   fields.push_back(*turns.rbegin());

   return fields;
}

} // namespace


///////////////////

Path::Path(const std::vector<FieldPos>& turns) : m_fields{generateFields(turns)}
{
}


std::optional<FieldPos> Path::first() const
{
   if (m_fields.empty())
      return std::nullopt;
   return m_fields[0];
}


std::optional<FieldPos> Path::next(const FieldPos& from) const
{
   auto pos = std::find(m_fields.begin(), m_fields.end(), from);
   if (pos == m_fields.end())
      throw std::runtime_error("Field not on path.");

   if (*pos == *m_fields.rbegin())
      return std::nullopt;

   return *++pos;
}
