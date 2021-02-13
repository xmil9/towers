//
// Dec-2020, Michael Lindner
// MIT license
//
#pragma once
#include "sge_animation.h"
#include <string>
#include <unordered_map>


namespace sge
{
///////////////////

class Animations
{
 public:
   void add(const std::string& tag, Animation&& a) { m_repos[tag] = std::move(a); }
   const Animation& operator[](const std::string& tag) const;
   void clear() { m_repos.clear(); }

 private:
   std::unordered_map<std::string, Animation> m_repos;
};

} // namespace sge
