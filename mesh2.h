//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "app_types.h"
#include <cstddef>
#include <vector>


class Mesh2
{
 public:
   using Coord = Point2_t::value_type;

 public:
   virtual ~Mesh2() = default;

   void set(const std::vector<Point2_t>& positions, const std::vector<Point2_t>& normals,
            const std::vector<VertexIdx>& indices);
   void set(std::vector<Point2_t>&& positions, std::vector<Point2_t>&& normals,
            std::vector<VertexIdx>&& indices);

   std::size_t numPositions() const { return m_positions.size(); }
   std::size_t numPositionsBytes() const { return m_positions.size() * sizeof(Point2_t); }
   const Coord* positions() const;

   std::size_t numNormals() const { return m_normals.size(); }
   std::size_t numNormalsBytes() const { return m_normals.size() * sizeof(Point2_t); }
   const Coord* normals() const;

   std::size_t numIndices() const { return m_indices.size(); }
   std::size_t numIndicesBytes() const { return m_indices.size() * sizeof(VertexIdx); }
   const VertexIdx* indices() const;

 protected:
   std::vector<Point2_t> m_positions;
   std::vector<Point2_t> m_normals;
   std::vector<VertexIdx> m_indices;
};


inline const Mesh2::Coord* Mesh2::positions() const
{
   return reinterpret_cast<const Coord*>(m_positions.data());
}

inline const Mesh2::Coord* Mesh2::normals() const
{
   return reinterpret_cast<const Coord*>(m_normals.data());
}

inline const VertexIdx* Mesh2::indices() const
{
   return m_indices.data();
}
