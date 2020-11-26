//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "terrain_types.h"
#include "glm/vec2.hpp"
#include <optional>
#include <vector>


///////////////////

class Path
{
 public:
   Path() = default;
   Path(const std::vector<FieldPos>& turns, glm::vec2 fieldSize);

   Rect start() const { return m_turns[0]; }
   std::optional<Rect> nextTurn(const Pos& at) const;

 private:
    // Terrain bounds of fields where the path takes a turn.
   std::vector<Rect> m_turns;
};
