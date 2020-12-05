//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "coords.h"
#include "glm/vec2.hpp"
#include "essentutils/fputil.h"
#include <stdexcept>


///////////////////

// Converts normalized coordinates ([0, 1]x[0, 1]) to pixel coordinates
// ([0, map width in pixels]x[0, map height in pixels]).
class MapCoordSys
{
 public:
   explicit MapCoordSys(PixDim mapSize);

   PixPos toRenderCoords(NormPos npos) const;
   NormPos toMapCoords(PixPos ppos) const;

 private:
   // Size of map in pixel coordinates, e.g. 800x600.
   PixDim m_mapSize;
};


inline MapCoordSys::MapCoordSys(PixDim mapSize) : m_mapSize{mapSize}
{
   if (m_mapSize.x == 0.f || m_mapSize.y == 0.f)
      throw std::runtime_error("Map of zero size is illegal.");
}

inline PixPos MapCoordSys::toRenderCoords(NormPos npos) const
{
   return npos * m_mapSize;
}

inline NormPos MapCoordSys::toMapCoords(PixPos ppos) const
{
   assert(m_mapSize.x != 0.f && m_mapSize.y != 0.f);
   return ppos / m_mapSize;
}
