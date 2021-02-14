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

   sge::PixPos toPix(sge::MapPos mpos) const;
   sge::MapPos toMap(sge::PixPos ppos) const;

   // Scales a source area into a destination area maintaining the source's aspect ratio.
   sge::MapDim scaleInto(sge::MapDim source, sge::MapDim dest) const;

 private:
   // Size of each field in pixel coordinates.
   sge::PixDim m_fieldSize{0.f, 0.f};
};


inline MapCoordSys::MapCoordSys(sge::PixDim fieldSize) : m_fieldSize{fieldSize}
{
   if (m_fieldSize.x == 0.f || m_fieldSize.y == 0.f)
      throw std::runtime_error("Fields of zero size are illegal.");
}

inline sge::PixPos MapCoordSys::toPix(sge::MapPos mpos) const
{
   return mpos * m_fieldSize;
}

inline sge::MapPos MapCoordSys::toMap(sge::PixPos ppos) const
{
   assert(m_fieldSize.x != 0.f && m_fieldSize.y != 0.f);
   return ppos / m_fieldSize;
}

inline sge::MapDim MapCoordSys::scaleInto(sge::MapDim source, sge::MapDim dest) const
{
   const auto ratio = std::min(dest.x / source.x, dest.y / source.y);
   return sge::MapDim{source.x * ratio, source.y * ratio};
}
