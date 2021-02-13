//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "sge_coords.h"
#include "glm/vec2.hpp"
#include <stdexcept>


///////////////////

// Converts map coordinates to pixel coordinates.
class MapCoordSys
{
 public:
   explicit MapCoordSys(sge::PixDim fieldSize);

   sge::PixPos toRenderCoords(sge::MapPos mpos) const;
   sge::MapPos toMapCoords(sge::PixPos ppos) const;

   // Returns the map size whose larger side is set to a given value and whose smaller
   // side is calculated to be at the same aspect ratio as a given source size.
   sge::MapDim scaleToSize(sge::MapCoord largerSide, sge::PixDim source) const;

 private:
   // Size of each field in pixel coordinates.
   sge::PixDim m_fieldSize{0.f, 0.f};
};


inline MapCoordSys::MapCoordSys(sge::PixDim fieldSize) : m_fieldSize{fieldSize}
{
   if (m_fieldSize.x == 0.f || m_fieldSize.y == 0.f)
      throw std::runtime_error("Fields of zero size are illegal.");
}

inline sge::PixPos MapCoordSys::toRenderCoords(sge::MapPos mpos) const
{
   return mpos * m_fieldSize;
}

inline sge::MapPos MapCoordSys::toMapCoords(sge::PixPos ppos) const
{
   assert(m_fieldSize.x != 0.f && m_fieldSize.y != 0.f);
   return ppos / m_fieldSize;
}

inline sge::MapDim MapCoordSys::scaleToSize(sge::MapCoord largerSide,
                                            sge::PixDim source) const
{
   const auto aspects = source.x >= source.y
                           ? std::make_pair<float, float>(1.f, source.y / source.x)
                           : std::make_pair<float, float>(source.x / source.y, 1.f);
   return sge::MapDim{largerSide * aspects.first, largerSide * aspects.second};
}
