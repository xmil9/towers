//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "coords.h"
#include "map_coord_sys.h"
#include "glm/vec2.hpp"
#include <cassert>
#include <optional>
#include <vector>


///////////////////

class Path
{
 public:
   using Index = std::size_t;

 public:
   Path() = default;
   Path(const std::vector<IntPos>& turns, NormDim fieldSize);

   std::size_t size() const { return m_turns.size(); }
   NormRect operator[](Index idx) const;
   NormRect start() const { return m_turns[0]; }
   NormRect finish() const;

 private:
   // Bounds (in map coords [0, 1]x[0, 1]) of fields where the path takes a turn.
   std::vector<NormRect> m_turns;
};


inline NormRect Path::operator[](Index idx) const
{
   assert(idx < size());
   return m_turns[idx];
}

inline NormRect Path::finish() const
{
   assert(size() > 0);
   return m_turns[size() - 1];
}


///////////////////

// Modifies a path by a given offset.
class OffsetPath
{
 public:
   OffsetPath(const Path* path, NormVec offset);

   std::size_t size() const;
   NormRect operator[](Path::Index idx) const;
   NormRect start() const;
   NormRect finish() const;

 private:
   NormRect transform(const NormRect& r) const;

 private:
   const Path* m_path;
   NormVec m_offset;
};


inline OffsetPath::OffsetPath(const Path* path, NormVec offset)
: m_path{path}, m_offset{offset}
{
   assert(m_path);
}

inline std::size_t OffsetPath::size() const
{
   return m_path->size();
}

inline NormRect OffsetPath::operator[](Path::Index idx) const
{
   return transform((*m_path)[idx]);
}

inline NormRect OffsetPath::start() const
{
   return transform(m_path->start());
}

inline NormRect OffsetPath::finish() const
{
   return transform(m_path->finish());
}

inline NormRect OffsetPath::transform(const NormRect& r) const
{
   return r + m_offset;
}
