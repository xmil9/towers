//
// Dec-2020, Michael Lindner
// MIT license
//
#include "explosion.h"


Explosion::Explosion(NormPos center, std::vector<Sprite> animation,
                     std::vector<int> durations, const MapCoordSys* cs)
: m_center{center}, m_animation{std::move(animation)}, m_durations{std::move(durations)},
  m_coordSys{cs}
{
   assert(m_animation.size() == m_durations.size());
   assert(!m_durations.empty());
}


void Explosion::render(const gll::Program& shaders)
{
   m_animation[m_stepIdx].render(shaders);
}


void Explosion::update()
{
   advanceAnimation();
}


bool Explosion::hasFinished() const
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


void Explosion::advanceAnimation()
{
   if (--m_stepDuration == 0)
   {
      if (++m_stepIdx < m_durations.size())
         m_stepDuration = m_durations[m_stepIdx];
   }
}
