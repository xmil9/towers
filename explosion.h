//
// Dec-2020, Michael Lindner
// MIT license
//
#pragma once
#include "map_coord_sys.h"
#include "sprite.h"
#include "glm/glm.hpp"

namespace gll
{
class Program;
}


///////////////////

class Explosion
{
 public:
   Explosion(NormPos center, std::vector<Sprite> animation, std::vector<int> durations,
             const MapCoordSys* cs);

   void render(const gll::Program& shaders);
   void update();
   bool hasFinished() const;

 private:
    void advanceAnimation();

 private:
   NormPos m_center;
   std::vector<Sprite> m_animation;
   std::vector<int> m_durations;
   std::size_t m_stepIdx = 0;
   int m_stepDuration = 0;
   const MapCoordSys* m_coordSys = nullptr;
   Sprite m_sprite;
};
