//
// Dec-2020, Michael Lindner
// MIT license
//
#pragma once
#include "map_coord_sys.h"
#include "sprite.h"
#include <vector>

namespace gll
{
class Program;
}


///////////////////

class Animation
{
 public:
   Animation(std::vector<Sprite> sprites, std::vector<int> durations,
             const MapCoordSys* cs);

   void render(const gll::Program& shaders);
   bool hasFinished() const;
   void setPosition(PixPos pos);

 private:
   void advance();

 private:
   std::vector<Sprite> m_sprites;
   // Duration (in frames) of how long each sprite is displayed.
   std::vector<int> m_durations;
   // Currently rendered step.
   std::size_t m_stepIdx = 0;
   // Duration left in step.
   int m_stepDuration = 0;
   const MapCoordSys* m_coordSys = nullptr;
};
