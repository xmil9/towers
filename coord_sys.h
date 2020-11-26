//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "terrain_types.h"
#include "glm/vec2.hpp"
#include <stdexcept>


class CoordSys
{
public:
   CoordSys() = default;
   explicit CoordSys(glm::vec2 terrainDim);

   glm::vec2 toRenderCoords(Pos terrain) const;
   Pos toTerrainCoords(glm::vec2 render) const;

private:
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
   assert(m_terrainDim.x != 0.f && m_terrainDim.y != 0.f);
   return terrain * m_terrainDim;
}

inline Pos CoordSys::toTerrainCoords(glm::vec2 render) const
{
   assert(m_terrainDim.x != 0.f && m_terrainDim.y != 0.f);
   return render / m_terrainDim;
}
