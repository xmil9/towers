//
// Nov-2020, Michael Lindner
// MIT license
//
#include "mesh2.h"
#include "gll_buffer.h"
#include "gll_data_format.h"
#include <algorithm>


void Mesh2::set(const std::vector<Point2_t>& positions,
                const std::vector<Point2_t>& normals,
                const std::vector<VertexIdx>& indices)
{
   m_positions.reserve(positions.size());
   std::copy(positions.begin(), positions.end(), m_positions.begin());
   
   m_normals.reserve(normals.size());
   std::copy(normals.begin(), normals.end(), m_normals.begin());
   
   m_indices.reserve(indices.size());
   std::copy(indices.begin(), indices.end(), m_indices.begin());
}


void Mesh2::set(std::vector<Point2_t>&& positions, std::vector<Point2_t>&& normals,
                std::vector<VertexIdx>&& indices)
{
   m_positions = std::move(positions);
   m_normals = std::move(normals);
   m_indices = std::move(indices);
}
