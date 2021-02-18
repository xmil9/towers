//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "spiel/coords.h"
#include <cassert>
#include <vector>


///////////////////

class Path
{
 public:
   using Index = std::size_t;

 public:
   Path() = default;
   explicit Path(const std::vector<sp::IntPos>& turns);

   std::size_t size() const { return m_turns.size(); }
   sp::MapPos operator[](Index idx) const;
   sp::MapPos start() const { return m_turns[0]; }
   sp::MapPos finish() const;

 private:
   // Map positions where the path takes a turn.
   std::vector<sp::MapPos> m_turns;
};


inline sp::MapPos Path::operator[](Index idx) const
{
   assert(idx < size());
   return m_turns[idx];
}

inline sp::MapPos Path::finish() const
{
   assert(size() > 0);
   return m_turns[size() - 1];
}


///////////////////

// Modifies a path by a given offset.
class OffsetPath
{
 public:
   OffsetPath(const Path* path, sp::MapVec offset);

   std::size_t size() const { return m_path->size(); }
   sp::MapPos operator[](Path::Index idx) const { return transform((*m_path)[idx]); }
   sp::MapPos start() const { return transform(m_path->start()); }
   sp::MapPos finish() const { return transform(m_path->finish()); }

 private:
   sp::MapPos transform(const sp::MapPos& pos) const { return pos + m_offset; }

 private:
   const Path* m_path = nullptr;
   sp::MapVec m_offset{0.f, 0.f};
};


inline OffsetPath::OffsetPath(const Path* path, sp::MapVec offset)
: m_path{path}, m_offset{offset}
{
   assert(m_path);
}
