//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "map_coords.h"
#include "glm/vec2.hpp"
#include <cassert>
#include <optional>
#include <vector>


///////////////////

class Path
{
 public:
   using Index = std::size_t;

 public:
   Path() = default;
   Path(const std::vector<FieldPos>& turns, glm::vec2 fieldSize);

   std::size_t size() const { return m_turns.size(); }
   MapRect operator[](Index idx) const;
   MapRect start() const { return m_turns[0]; }
   MapRect finish() const;
   std::optional<MapRect> nextTurn(const MapPos& at) const;

 private:
   // Bounds (in map coords [0, 1]x[0, 1]) of fields where the path takes a turn.
   std::vector<MapRect> m_turns;
};


inline MapRect Path::operator[](Index idx) const
{
   assert(idx < size());
   return m_turns[idx];
}

inline MapRect Path::finish() const
{
   assert(size() > 0);
   return m_turns[size() - 1];
}
