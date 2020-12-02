//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "terrain_types.h"
#include "glm/vec2.hpp"
#include <stdexcept>


///////////////////

// Maps terrain coordinates ([0, 1]x[0, 1]) to render coordinates
// ([0, terrain width]x[0, terrain height]).
class CoordSys
{
public:
   explicit CoordSys(glm::vec2 terrainDim);

   glm::vec2 toRenderCoords(Pos terrain) const;
   Pos toTerrainCoords(glm::vec2 render) const;

private:
   // Size of terrain in render coordinates, e.g. 800x600.
   glm::vec2 m_terrainDim;
};


inline CoordSys::CoordSys(glm::vec2 terrainDim)
   : m_terrainDim{terrainDim}
{
   if (m_terrainDim.x == 0.f || m_terrainDim.y == 0.f)
      throw std::runtime_error("Terrain of zero size is illegal.");
}

inline glm::vec2 CoordSys::toRenderCoords(Pos terrain) const
{
   return terrain * m_terrainDim;
}

inline Pos CoordSys::toTerrainCoords(glm::vec2 render) const
{
   assert(m_terrainDim.x != 0.f && m_terrainDim.y != 0.f);
   return render / m_terrainDim;
}
