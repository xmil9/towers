//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "coords.h"
#include "glm/vec2.hpp"
#include <stdexcept>


///////////////////

// Converts map coordinates to pixel coordinates.
class MapCoordSys
{
 public:
   explicit MapCoordSys(PixDim fieldSize);

   PixPos toRenderCoords(MapPos mpos) const;
   MapPos toMapCoords(PixPos ppos) const;

   // Returns the map size whose larger side is set to a given value and whose smaller
   // side is calculated to be at the same aspect ratio as a given source size.
   MapDim scaleToSize(MapCoord largerSide, PixDim source) const;

 private:
   // Size of each field in pixel coordinates.
   PixDim m_fieldSize{0.f, 0.f};
};


inline MapCoordSys::MapCoordSys(PixDim fieldSize) : m_fieldSize{fieldSize}
{
   if (m_fieldSize.x == 0.f || m_fieldSize.y == 0.f)
      throw std::runtime_error("Fields of zero size are illegal.");
}

inline PixPos MapCoordSys::toRenderCoords(MapPos mpos) const
{
   return mpos * m_fieldSize;
}

inline MapPos MapCoordSys::toMapCoords(PixPos ppos) const
{
   assert(m_fieldSize.x != 0.f && m_fieldSize.y != 0.f);
   return ppos / m_fieldSize;
}

inline MapDim MapCoordSys::scaleToSize(MapCoord largerSide, PixDim source) const
{
   const auto aspects = source.x >= source.y
                           ? std::make_pair<float, float>(1.f, source.y / source.x)
                           : std::make_pair<float, float>(source.x / source.y, 1.f);
   return MapDim{largerSide * aspects.first, largerSide * aspects.second};
}
