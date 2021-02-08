//
// Dec-2020, Michael Lindner
// MIT license
//
#pragma once
#include "coords.h"
#include <functional>
#include <string>
#include <unordered_map>

namespace sge
{
class Animation;
}


class AnimationFactory
{
 public:
   AnimationFactory();

   sge::Animation make(const std::string& tag, sge::PixDim size);

 private:
   using FactoryFunc = std::function<sge::Animation(sge::PixDim)>;

 private:
   std::unordered_map<std::string, FactoryFunc> m_factories;
};
