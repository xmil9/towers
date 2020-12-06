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

   // Returns the map size whose larger side is set to a given value and whose smaller
   // side is calculated to be at the same aspect ratio as a given render size.
   NormDim makeEquivalentMapSize(NormCoord largerSide, PixDim source) const;

 private:
   float mapAspect() const { return m_mapSize.x / m_mapSize.y; }

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

inline NormDim MapCoordSys::makeEquivalentMapSize(NormCoord largerSide,
                                                  PixDim source) const
{
   // We also need to account for the aspect ratio of the map itself otherwise the shorter
   // size would be relative to the larger map size instead of the shorter map size that
   // it is supposed relative to.
   const float ma = mapAspect();
   const auto aspects =
      source.x >= source.y
         ? std::make_pair<float, float>(1.f, source.y / source.x * ma)
         : std::make_pair<float, float>(source.x / source.y * (1.f / ma), 1.f);
   return NormDim{largerSide * aspects.first, largerSide * aspects.second};
}
