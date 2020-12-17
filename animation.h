//
// Dec-2020, Michael Lindner
// MIT license
//
#pragma once
#include "map_coord_sys.h"
#include "sprite.h"
#include <memory>
#include <optional>
#include <vector>

namespace gll
{
class Program;
}


///////////////////

// Animation of a sequence of sprites.
// Keeps track of currently rendered frame and the position where it's rendered.
class Animation
{
 public:
   Animation(std::vector<Sprite> sprites, std::vector<int> frames, bool repeat,
             const MapCoordSys* cs, NormPos center = {});

   void render(const gll::Program& shaders);
   int numFrames() const { return m_totalFrames; }
   bool hasFinished() const { return m_currFrame >= numFrames(); }
   // Returns the size of the sprite rendered for the given frame.
   PixDim size(int frame) const;
   void setPosition(NormPos center) { m_center = center; }

 private:
   void populateMaxFrameIndices();
   std::optional<std::size_t> calcSpriteIndex(int frame) const;
   void reset();

 private:
   std::vector<Sprite> m_sprites;
   // Number of frames for how long each sprite is displayed.
   std::vector<int> m_frames;
   // Repeat the animation after it finishes.
   bool m_repeat = false;
   int m_totalFrames = 0;
   // The index of the last frame in each step.
   // Helps to find the step that a frame index falls into.
   std::vector<int> m_maxFrameIdx;
   int m_currFrame = 0;
   const MapCoordSys* m_coordSys = nullptr;
   NormPos m_center;
};
