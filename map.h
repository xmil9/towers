//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "map_data.h"
#include "glm/vec2.hpp"


class Map
{
public:
   explicit Map(MapData&& data);

   const Path& path() const { return m_rep.paths[0]; }

private:
   MapData m_rep;
};


inline Map::Map(MapData&& data)
   : m_rep{std::move(data)}
{
}
