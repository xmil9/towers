//
// Dec-2020, Michael Lindner
// MIT license
//
#include "animation.h"
#include <algorithm>
#include <iterator>
#include <numeric>


///////////////////

Animation::Animation(std::vector<Sprite> sprites, std::vector<int> frames, bool repeat,
                     const MapCoordSys* cs, NormPos center)
: m_sprites{std::move(sprites)}, m_frames{std::move(frames)}, m_repeat{repeat},
  m_totalFrames{std::accumulate(m_frames.begin(), m_frames.end(), 0)},
  m_coordSys{cs}, m_center{center}
{
   assert(m_frames.size() == m_sprites.size());
   populateMaxFrameIndices();
   assert(m_coordSys);
}


void Animation::render(const gll::Program& shaders)
{
   if (m_totalFrames == 0)
      return;

   if (hasFinished() && m_repeat)
      reset();

   if (!hasFinished())
   {
      const auto idx = calcSpriteIndex(m_currFrame);
      if (idx)
         m_sprites[*idx].render(shaders, m_coordSys->toRenderCoords(m_center) -
                                            size(m_currFrame) / 2.f);
      ++m_currFrame;
   }
}


PixDim Animation::size(int frame) const
{
   const auto idx = calcSpriteIndex(frame);
   return idx ? m_sprites[*idx].size() : PixDim();
}


void Animation::setRotation(Angle_t rot)
{
   for (auto& sp : m_sprites)
      sp.setRotation(rot);
}


void Animation::populateMaxFrameIndices()
{
   m_maxFrameIdx.reserve(m_frames.size());
   std::partial_sum(m_frames.begin(), m_frames.end(), std::back_inserter(m_maxFrameIdx));
}


std::optional<std::size_t> Animation::calcSpriteIndex(int frame) const
{
   const auto pos = std::lower_bound(m_maxFrameIdx.begin(), m_maxFrameIdx.end(), frame);
   if (pos != m_maxFrameIdx.end())
      return pos - m_maxFrameIdx.begin();
   return std::nullopt;
}


void Animation::reset()
{
   m_currFrame = 0;
}
