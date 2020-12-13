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

// Sequence of animated sprites over a number of frames.
// Immutable to allow sharing.
class AnimationSeq
{
 public:
   AnimationSeq(std::vector<Sprite> sprites, std::vector<int> frames);

   void render(const gll::Program& shaders, PixPos at, int frame) const;
   int numFrames() const { return m_totalFrames; }
   // Returns the size of the sprite rendered for the given frame.
   PixDim size(int frame) const;

 private:
   void populateMaxFrameIndices();
   std::optional<std::size_t> calcSpriteIndex(int frame) const;

 private:
   std::vector<Sprite> m_sprites;
   // Number of frames for how long each sprite is displayed.
   std::vector<int> m_frames;
   int m_totalFrames = 0;
   // The index of the last frame in each step.
   // Helps to find the step that a frame index falls into.
   std::vector<int> m_maxFrameIdx;
};


///////////////////

// Animation of a sequence of sprites.
// Keeps track of currently rendered frame and the position where it's rendered.
class Animation
{
 public:
   Animation(std::shared_ptr<AnimationSeq> seq, const MapCoordSys* cs,
             NormPos center = {});

   void render(const gll::Program& shaders);
   bool hasFinished() const { return m_currFrame >= m_seq->numFrames(); }
   void setPosition(NormPos center) { m_center = center; }

 private:
   std::shared_ptr<AnimationSeq> m_seq;
   const MapCoordSys* m_coordSys = nullptr;
   NormPos m_center;
   int m_currFrame = 0;
};


inline Animation::Animation(std::shared_ptr<AnimationSeq> seq, const MapCoordSys* cs,
                            NormPos center)
: m_seq{seq}, m_coordSys{cs}, m_center{center}
{
   assert(m_seq);
   assert(m_coordSys);
}
