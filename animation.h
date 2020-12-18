//
// Dec-2020, Michael Lindner
// MIT license
//
#pragma once
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
   Animation() = default;
   Animation(std::vector<Sprite> sprites, std::vector<int> frames, bool repeat);

   void render(const gll::Program& shaders, PixPos atLeftTop);
   int numFrames() const { return m_totalFrames; }
   bool hasFinished() const { return m_currFrame >= numFrames(); }
   // Returns the size of the current frame's sprite.
   PixDim size() const { return size(m_currFrame); }

   void setRotation(Angle_t rot);
   // Sets size of all sprites in the animation.
   void setSize(PixDim size);
   void scale(float factor);
   void rotate(Angle_t rot);

 private:
   void populateMaxFrameIndices();
   // Returns the size of the sprite rendered for the given frame.
   PixDim size(int frame) const;
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
};
