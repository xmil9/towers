//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "spiel/coords.h"
#include "glm/vec2.hpp"
#include <stdexcept>


///////////////////

// Converts map coordinates to pixel coordinates.
class MapCoordSys
{
 public:
   explicit MapCoordSys(sp::PixDim fieldSize);

   sp::PixPos toPix(sp::MapPos mpos) const;
   sp::MapPos toMap(sp::PixPos ppos) const;

   // Scales a source area into a destination area maintaining the source's aspect ratio.
   sp::MapDim scaleInto(sp::MapDim source, sp::MapDim dest) const;

 private:
   // Size of each field in pixel coordinates.
   sp::PixDim m_fieldSize{0.f, 0.f};
};


inline MapCoordSys::MapCoordSys(sp::PixDim fieldSize) : m_fieldSize{fieldSize}
{
   if (m_fieldSize.x == 0.f || m_fieldSize.y == 0.f)
      throw std::runtime_error("Fields of zero size are illegal.");
}

inline sp::PixPos MapCoordSys::toPix(sp::MapPos mpos) const
{
   return mpos * m_fieldSize;
}

inline sp::MapPos MapCoordSys::toMap(sp::PixPos ppos) const
{
   assert(m_fieldSize.x != 0.f && m_fieldSize.y != 0.f);
   return ppos / m_fieldSize;
}

inline sp::MapDim MapCoordSys::scaleInto(sp::MapDim source, sp::MapDim dest) const
{
   const auto ratio = std::min(dest.x / source.x, dest.y / source.y);
   return sp::MapDim{source.x * ratio, source.y * ratio};
}
