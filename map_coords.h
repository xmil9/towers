//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "render_coords.h"
#include "glm/vec2.hpp"
#include "essentutils/fputil.h"
#include <stdexcept>


///////////////////

// Position in map coordinates. Coordinates are in range [0.0, 1.0].
using MapPos = glm::vec2;

inline bool isEqualPos(MapPos a, MapPos b)
{
   return sutil::equal(a.x, b.x) && sutil::equal(a.y, b.y);
}

// Dimensions in map coordinates.
using MapDim = glm::vec2;


///////////////////

struct MapRect
{
   MapPos lt;
   MapPos rb;

   bool contains(MapPos at) const;
   MapPos center() const;
};

inline bool MapRect::contains(MapPos at) const
{
   return lt.x <= at.x && at.x < rb.x && lt.y <= at.y && at.y < rb.y;
}

inline MapPos MapRect::center() const
{
   return {(lt.x + rb.x) / 2.f, (lt.y + rb.y) / 2.f};
}


///////////////////

// Indices of a field.
using FieldPos = glm::ivec2;


///////////////////

// Converts map coordinates ([0, 1]x[0, 1]) to render coordinates
// ([0, map width in pixels]x[0, map height in pixels]).
class MapCoordSys
{
public:
   explicit MapCoordSys(RenderDim rdim);

   RenderPos toRenderCoords(MapPos mpos) const;
   MapPos toMapCoords(RenderPos rpos) const;

private:
   // Size of map in render coordinates, e.g. 800x600.
   RenderDim m_renderDim;
};


inline MapCoordSys::MapCoordSys(RenderDim rdim)
   : m_renderDim{rdim}
{
   if (m_renderDim.x == 0.f || m_renderDim.y == 0.f)
      throw std::runtime_error("Map of zero size is illegal.");
}

inline RenderPos MapCoordSys::toRenderCoords(MapPos mpos) const
{
   return mpos * m_renderDim;
}

inline MapPos MapCoordSys::toMapCoords(RenderPos rpos) const
{
   assert(m_renderDim.x != 0.f && m_renderDim.y != 0.f);
   return rpos / m_renderDim;
}
