//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "sge_coords.h"
#include <cassert>
#include <vector>


///////////////////

class Path
{
 public:
   using Index = std::size_t;

 public:
   Path() = default;
   explicit Path(const std::vector<sge::IntPos>& turns);

   std::size_t size() const { return m_turns.size(); }
   sge::MapPos operator[](Index idx) const;
   sge::MapPos start() const { return m_turns[0]; }
   sge::MapPos finish() const;

 private:
   // Map positions where the path takes a turn.
   std::vector<sge::MapPos> m_turns;
};


inline sge::MapPos Path::operator[](Index idx) const
{
   assert(idx < size());
   return m_turns[idx];
}

inline sge::MapPos Path::finish() const
{
   assert(size() > 0);
   return m_turns[size() - 1];
}


///////////////////

// Modifies a path by a given offset.
class OffsetPath
{
 public:
   OffsetPath(const Path* path, sge::MapVec offset);

   std::size_t size() const { return m_path->size(); }
   sge::MapPos operator[](Path::Index idx) const { return transform((*m_path)[idx]); }
   sge::MapPos start() const { return transform(m_path->start()); }
   sge::MapPos finish() const { return transform(m_path->finish()); }

 private:
   sge::MapPos transform(const sge::MapPos& pos) const { return pos + m_offset; }

 private:
   const Path* m_path = nullptr;
   sge::MapVec m_offset{0.f, 0.f};
};


inline OffsetPath::OffsetPath(const Path* path, sge::MapVec offset)
: m_path{path}, m_offset{offset}
{
   assert(m_path);
}
