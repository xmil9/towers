//
// Dec-2020, Michael Lindner
// MIT license
//
#include "animation.h"


///////////////////

Animation::Animation(std::vector<Sprite> sprites, std::vector<int> durations,
                     const MapCoordSys* cs)
: m_sprites{std::move(sprites)}, m_durations{std::move(durations)}, m_coordSys{cs}
{
   if (m_sprites.empty() || m_durations.empty() || m_durations.size() != m_sprites.size())
      throw std::runtime_error("Misconfigured animation.");

   m_stepDuration = m_durations[0];
}


void Animation::render(const gll::Program& shaders)
{
   if (!hasFinished())
   {
      m_sprites[m_stepIdx].render(shaders);
      advance();
   }
}


bool Animation::hasFinished() const
{
   // Past the last step.
   if (m_stepIdx >= m_durations.size())
      return true;
   // Duration of last step has passed.
   const std::size_t LastStepIdx = m_durations.size() - 1;
   if (m_stepIdx == LastStepIdx)
      return m_stepDuration == 0;
   return false;
}


void Animation::setPosition(PixPos pos)
{
   for (auto& sprite : m_sprites)
      sprite.setPosition(pos);
}


void Animation::advance()
{
   if (--m_stepDuration == 0)
   {
      if (++m_stepIdx < m_durations.size())
         m_stepDuration = m_durations[m_stepIdx];
   }
}
