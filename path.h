//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "map_types.h"
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
   Rect operator[](Index idx) const;
   Rect start() const { return m_turns[0]; }
   Rect finish() const;
   std::optional<Rect> nextTurn(const Pos& at) const;

 private:
   // Bounds (in map coords [0, 1]x[0, 1]) of fields where the path takes a turn.
   std::vector<Rect> m_turns;
};


inline Rect Path::operator[](Index idx) const
{
   assert(idx < size());
   return m_turns[idx];
}

inline Rect Path::finish() const
{
   assert(size() > 0);
   return m_turns[size() - 1];
}
