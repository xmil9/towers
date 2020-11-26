//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "terrain_data.h"
#include "glm/vec2.hpp"


class Terrain
{
public:
   explicit Terrain(TerrainData&& data);

   const Path& path() const { return m_rep.paths[0]; }

private:
   TerrainData m_rep;
};


inline Terrain::Terrain(TerrainData&& data)
   : m_rep{std::move(data)}
{
}
