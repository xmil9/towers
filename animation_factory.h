//
// Dec-2020, Michael Lindner
// MIT license
//
#pragma once
#include "coords.h"
#include <functional>
#include <string>
#include <unordered_map>

class Animation;


class AnimationFactory
{
 public:
   AnimationFactory();

   Animation make(const std::string& tag, PixDim size);

 private:
   using FactoryFunc = std::function<Animation(PixDim)>;

 private:
   std::unordered_map<std::string, FactoryFunc> m_factories;
};
