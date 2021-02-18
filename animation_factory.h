//
// Dec-2020, Michael Lindner
// MIT license
//
#pragma once
#include "spiel/coords.h"
#include <functional>
#include <string>
#include <unordered_map>

namespace sp
{
class Animation;
}


class AnimationFactory
{
 public:
   AnimationFactory();

   sp::Animation make(const std::string& tag, sp::PixDim size);

 private:
   using FactoryFunc = std::function<sp::Animation(sp::PixDim)>;

 private:
   std::unordered_map<std::string, FactoryFunc> m_factories;
};
