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
class MapCoordSys;
class SpriteRenderer;


class AnimationFactory
{
 public:
   AnimationFactory(SpriteRenderer* renderer, const MapCoordSys* cs);

   Animation make(const std::string& tag, PixDim size);

 private:
   using FactoryFunc =
      std::function<Animation(PixDim size, SpriteRenderer*, const MapCoordSys*)>;

 private:
   SpriteRenderer* m_renderer = nullptr;
   const MapCoordSys* m_coordSys = nullptr;
   std::unordered_map<std::string, FactoryFunc> m_factories;
};
