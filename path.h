//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "coords.h"
#include <cassert>
#include <vector>


///////////////////

class Path
{
 public:
   using Index = std::size_t;

 public:
   Path() = default;
   explicit Path(const std::vector<IntPos>& turns);

   std::size_t size() const { return m_turns.size(); }
   MapPos operator[](Index idx) const;
   MapPos start() const { return m_turns[0]; }
   MapPos finish() const;

 private:
   // Map positions where the path takes a turn.
   std::vector<MapPos> m_turns;
};


inline MapPos Path::operator[](Index idx) const
{
   assert(idx < size());
   return m_turns[idx];
}

inline MapPos Path::finish() const
{
   assert(size() > 0);
   return m_turns[size() - 1];
}


///////////////////

// Modifies a path by a given offset.
class OffsetPath
{
 public:
   OffsetPath(const Path* path, MapVec offset);

   std::size_t size() const { return m_path->size(); }
   MapPos operator[](Path::Index idx) const { return transform((*m_path)[idx]); }
   MapPos start() const { return transform(m_path->start()); }
   MapPos finish() const { return transform(m_path->finish()); }

 private:
   MapPos transform(const MapPos& pos) const { return pos + m_offset; }

 private:
   const Path* m_path = nullptr;
   MapVec m_offset{0.f, 0.f};
};


inline OffsetPath::OffsetPath(const Path* path, MapVec offset)
: m_path{path}, m_offset{offset}
{
   assert(m_path);
}
