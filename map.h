//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "map_data.h"
#include "glm/vec2.hpp"
#include <cassert>


///////////////////

class Map
{
 public:
   explicit Map(MapData&& data);

   sge::IntDim sizeInFields() const { return m_rep.mapSize; }
   const Path& path() const { return m_rep.paths[0]; }
   bool isOnMap(sge::MapPos pos) const;
   bool canBuildOnField(sge::MapPos pos) const;
   bool isOnSameField(sge::MapPos a, sge::MapPos b) const;

 private:
   int fieldIndex(sge::MapPos pos) const;

 private:
   MapData m_rep;
};


inline Map::Map(MapData&& data) : m_rep{std::move(data)}
{
}

inline bool Map::isOnMap(sge::MapPos pos) const
{
   return pos.x >= 0.f && pos.x < sizeInFields().x && pos.y >= 0.f &&
          pos.y < sizeInFields().y;
}

inline bool Map::canBuildOnField(sge::MapPos pos) const
{
   const int fieldIdx = fieldIndex(pos);
   assert(fieldIdx >= 0 && fieldIdx < m_rep.terrain.size());
   return m_rep.terrain[fieldIdx].canBuildOn();
}

inline bool Map::isOnSameField(sge::MapPos a, sge::MapPos b) const
{
   return fieldIndex(a) == fieldIndex(b);
}

inline int Map::fieldIndex(sge::MapPos pos) const
{
   const int col = static_cast<int>(pos.x);
   const int row = static_cast<int>(pos.y);
   return row * sizeInFields().x + col;
}
