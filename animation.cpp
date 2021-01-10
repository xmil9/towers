//
// Dec-2020, Michael Lindner
// MIT license
//
#include "animation.h"
#include <algorithm>
#include <iterator>
#include <numeric>


///////////////////

Animation::Animation(std::vector<Sprite> sprites, std::vector<int> frames, bool repeat)
: m_sprites{std::move(sprites)}, m_frames{std::move(frames)}, m_repeat{repeat},
  m_totalFrames{std::accumulate(m_frames.begin(), m_frames.end(), 0)}
{
   assert(m_frames.size() == m_sprites.size());
   populateMaxFrameIndices();
}


Animation& Animation::setRotation(Angle_t rot)
{
   for (auto& sp : m_sprites)
      sp.setRotation(rot);
   return *this;
}


Animation& Animation::setSize(PixDim size)
{
   for (auto& sp : m_sprites)
      sp.setSize(size);
   return *this;
}


Animation& Animation::scale(float factor)
{
   for (auto& sp : m_sprites)
      sp.scale(factor);
   return *this;
}


Animation& Animation::rotate(Angle_t rot)
{
   for (auto& sp : m_sprites)
      sp.rotate(rot);
   return *this;
}


std::optional<const Sprite*> Animation::nextFrame()
{
   if (m_totalFrames == 0 || (hasFinished() && !m_repeat))
      return std::nullopt;

   if (m_repeat)
      reset();

   const auto idx = calcSpriteIndex(m_currFrame++);
   return idx ? std::make_optional(&m_sprites[*idx]) : std::nullopt;
}


void Animation::populateMaxFrameIndices()
{
   m_maxFrameIdx.reserve(m_frames.size());
   std::partial_sum(m_frames.begin(), m_frames.end(), std::back_inserter(m_maxFrameIdx));
}


PixDim Animation::size(int frame) const
{
   const auto idx = calcSpriteIndex(frame);
   return idx ? m_sprites[*idx].size() : PixDim();
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
