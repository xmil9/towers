//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "coords.h"
#include "point2.h"
#include "opengl_util/gll_data_format.h"
#include <algorithm>
#include <cstddef>
#include <vector>


namespace sp
{
///////////////////

class Mesh2
{
 public:
   using Point = Point2<NormCoord>;
   using VertexIdx = unsigned int;

 public:
   virtual ~Mesh2() = default;

   std::size_t numPositions() const { return m_positions.size(); }
   std::size_t numPositionBytes() const { return m_positions.size() * sizeof(Point); }
   const NormCoord* positions() const;
   constexpr gll::DataFormat positionsFormat() const;
   void setPositions(const std::vector<Point>& positions);
   void setPositions(std::vector<Point>&& positions);

   std::size_t numIndices() const { return m_indices.size(); }
   std::size_t numIndexBytes() const { return m_indices.size() * sizeof(VertexIdx); }
   const VertexIdx* indices() const { return m_indices.data(); }
   void setIndices(const std::vector<VertexIdx>& indices);
   void setIndices(std::vector<VertexIdx>&& indices);

   std::size_t numTextureCoords() const { return m_texCoords.size(); }
   std::size_t numTextureCoordBytes() const;
   const NormCoord* textureCoords() const;
   constexpr gll::DataFormat textureCoordsFormat() const;
   void setTextureCoords(const std::vector<Point>& texCoords);
   void setTextureCoords(std::vector<Point>&& texCoords);

 protected:
   std::vector<Point> m_positions;
   std::vector<VertexIdx> m_indices;
   std::vector<Point> m_texCoords;
};


inline const NormCoord* Mesh2::positions() const
{
   return reinterpret_cast<const NormCoord*>(m_positions.data());
}

inline constexpr gll::DataFormat Mesh2::positionsFormat() const
{
   return {2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr};
}

inline void Mesh2::setPositions(const std::vector<Point>& positions)
{
   m_positions.resize(positions.size());
   std::copy(positions.begin(), positions.end(), m_positions.begin());
}

inline void Mesh2::setPositions(std::vector<Point>&& positions)
{
   m_positions = std::move(positions);
}

inline void Mesh2::setIndices(const std::vector<VertexIdx>& indices)
{
   m_indices.resize(indices.size());
   std::copy(indices.begin(), indices.end(), m_indices.begin());
}

inline void Mesh2::setIndices(std::vector<VertexIdx>&& indices)
{
   m_indices = std::move(indices);
}

inline std::size_t Mesh2::numTextureCoordBytes() const
{
   return m_texCoords.size() * sizeof(Point);
}

inline const NormCoord* Mesh2::textureCoords() const
{
   return reinterpret_cast<const NormCoord*>(m_texCoords.data());
}

inline constexpr gll::DataFormat Mesh2::textureCoordsFormat() const
{
   return {2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr};
}

inline void Mesh2::setTextureCoords(const std::vector<Point>& texCoords)
{
   m_texCoords.resize(texCoords.size());
   std::copy(texCoords.begin(), texCoords.end(), m_texCoords.begin());
}

inline void Mesh2::setTextureCoords(std::vector<Point>&& texCoords)
{
   m_texCoords = std::move(texCoords);
}

} // namespace sp
