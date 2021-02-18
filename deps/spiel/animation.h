//
// Dec-2020, Michael Lindner
// MIT license
//
#pragma once
#include "coords.h"
#include "sprite.h"
#include <optional>
#include <vector>


namespace sp
{
///////////////////

// Animates of a sequence of sprites.
// Keeps track of currently rendered frame.
class Animation
{
 public:
   Animation() = default;
   Animation(std::vector<Sprite> sprites, std::vector<int> frames, bool repeat);

   // Returns the size of the current frame's sprite.
   PixDim size() const { return size(m_currFrame); }

   Animation& setRotation(Angle rot);
   // Sets size of all sprites in the animation.
   Animation& setSize(PixDim size);
   Animation& scale(float factor);
   Animation& rotate(Angle rot);

   bool hasFinished() const { return m_currFrame >= m_totalFrames; }
   std::optional<const Sprite*> currentFrame();
   std::optional<const Sprite*> nextFrame();

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

} // namespace sp
