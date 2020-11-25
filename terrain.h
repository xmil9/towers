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
   Terrain() = default;
   Terrain(TerrainData&& data, glm::ivec2 fieldSize);

   const Path& path() const { return m_rep.paths[0]; }
   glm::ivec2 fieldSize() const { return m_fieldSize; }

private:
   TerrainData m_rep;
   // Size of each field in pixels.
   glm::ivec2 m_fieldSize;
};


inline Terrain::Terrain(TerrainData&& data, glm::ivec2 fieldSize)
   : m_rep{std::move(data)}, m_fieldSize{fieldSize}
{
}
