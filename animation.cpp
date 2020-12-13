//
// Dec-2020, Michael Lindner
// MIT license
//
#include "animation.h"
#include <algorithm>
#include <iterator>
#include <numeric>


///////////////////

AnimationSeq::AnimationSeq(std::vector<Sprite> sprites, std::vector<int> frames)
: m_sprites{std::move(sprites)}, m_frames{std::move(frames)},
  m_totalFrames{std::accumulate(m_frames.begin(), m_frames.end(), 0)}
{
   if (m_sprites.empty() || m_frames.empty() || m_frames.size() != m_sprites.size())
      throw std::runtime_error("Misconfigured animation.");

   populateMaxFrameIndices();
}


void AnimationSeq::render(const gll::Program& shaders, PixPos at, int frame) const
{
   const auto idx = calcSpriteIndex(frame);
   if (idx)
      m_sprites[*idx].render(shaders, at);
}


PixDim AnimationSeq::size(int frame) const
{
   const auto idx = calcSpriteIndex(frame);
   return idx ? m_sprites[*idx].size() : PixDim();
}


void AnimationSeq::populateMaxFrameIndices()
{
   m_maxFrameIdx.reserve(m_frames.size());
   std::partial_sum(m_frames.begin(), m_frames.end(), std::back_inserter(m_maxFrameIdx));
}


std::optional<std::size_t> AnimationSeq::calcSpriteIndex(int frame) const
{
   const auto pos = std::lower_bound(m_maxFrameIdx.begin(), m_maxFrameIdx.end(), frame);
   if (pos != m_maxFrameIdx.end())
      return pos - m_maxFrameIdx.begin();
   return std::nullopt;
}


///////////////////

void Animation::render(const gll::Program& shaders)
{
   if (!hasFinished())
   {
      m_seq->render(shaders,
                    m_coordSys->toRenderCoords(m_center) - m_seq->size(m_currFrame) / 2.f,
                    m_currFrame);
      ++m_currFrame;
   }
}
