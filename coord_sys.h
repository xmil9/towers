//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "map_types.h"
#include "glm/vec2.hpp"
#include <stdexcept>


///////////////////

// Converts map coordinates ([0, 1]x[0, 1]) to render coordinates
// ([0, map width]x[0, map height]).
class CoordSys
{
public:
   explicit CoordSys(glm::vec2 renderDim);

   glm::vec2 toRenderCoords(Pos mapPos) const;
   Pos toMapCoords(glm::vec2 renderPos) const;

private:
   // Size of map in render coordinates, e.g. 800x600.
   glm::vec2 m_renderDim;
};


inline CoordSys::CoordSys(glm::vec2 renderDim)
   : m_renderDim{renderDim}
{
   if (m_renderDim.x == 0.f || m_renderDim.y == 0.f)
      throw std::runtime_error("Map of zero size is illegal.");
}

inline glm::vec2 CoordSys::toRenderCoords(Pos mapPos) const
{
   return mapPos * m_renderDim;
}

inline Pos CoordSys::toMapCoords(glm::vec2 renderPos) const
{
   assert(m_renderDim.x != 0.f && m_renderDim.y != 0.f);
   return renderPos / m_renderDim;
}
