//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "glm/vec2.hpp"
#include "essentutils/fputil.h"
#include <stdexcept>


///////////////////

// Position in map coordinates. Coordinates are in range [0.0, 1.0].
using Pos = glm::vec2;

inline bool isEqualPos(Pos a, Pos b)
{
   return sutil::equal(a.x, b.x) && sutil::equal(a.y, b.y);
}


///////////////////

struct Rect
{
   Pos lt;
   Pos rb;

   bool contains(Pos at) const;
   Pos center() const;
};

inline bool Rect::contains(Pos at) const
{
   return lt.x <= at.x && at.x < rb.x && lt.y <= at.y && at.y < rb.y;
}

inline Pos Rect::center() const
{
   return {(lt.x + rb.x) / 2.f, (lt.y + rb.y) / 2.f};
}


///////////////////

// Indices of a field.
using FieldPos = glm::ivec2;


///////////////////

// Converts map coordinates ([0, 1]x[0, 1]) to render coordinates
// ([0, map width]x[0, map height]).
class MapCoordSys
{
public:
   explicit MapCoordSys(glm::vec2 renderDim);

   glm::vec2 toRenderCoords(Pos mapPos) const;
   Pos toMapCoords(glm::vec2 renderPos) const;

private:
   // Size of map in render coordinates, e.g. 800x600.
   glm::vec2 m_renderDim;
};


inline MapCoordSys::MapCoordSys(glm::vec2 renderDim)
   : m_renderDim{renderDim}
{
   if (m_renderDim.x == 0.f || m_renderDim.y == 0.f)
      throw std::runtime_error("Map of zero size is illegal.");
}

inline glm::vec2 MapCoordSys::toRenderCoords(Pos mapPos) const
{
   return mapPos * m_renderDim;
}

inline Pos MapCoordSys::toMapCoords(glm::vec2 renderPos) const
{
   assert(m_renderDim.x != 0.f && m_renderDim.y != 0.f);
   return renderPos / m_renderDim;
}
