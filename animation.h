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
   Animation(std::vector<Sprite> sprites, std::vector<int> durations);

   void render(const gll::Program& shaders, PixPos at);
   bool hasFinished() const;
   // Returns the size of the current sprite.
   PixDim size() const;
   void scale(float factor);
   void rotate(Angle_t rot);

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
};
