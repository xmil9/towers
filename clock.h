//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include <chrono>


template <typename Duration = std::chrono::microseconds> class Clock
{
 public:
   template <typename T> T measureCycle();
   template <typename T> T lastCycleLength() const;
   template <typename T> T elapsed() const;

 private:
   using TimePoint = std::chrono::steady_clock::time_point;

 private:
   std::chrono::steady_clock m_clock;
   TimePoint m_lastCycle;
   Duration m_cycleLength;
};


template <typename Duration> template <typename T> T Clock<Duration>::measureCycle()
{
   const TimePoint now = m_clock.now();
   m_cycleLength = std::chrono::duration_cast<Duration>(now - m_lastCycle);
   m_lastCycle = now;
   return lastCycleLength<T>();
}


template <typename Duration>
template <typename T>
T Clock<Duration>::lastCycleLength() const
{
   return static_cast<T>(m_cycleLength.count());
}


template <typename Duration> template <typename T> T Clock<Duration>::elapsed() const
{
   const auto elapsed = std::chrono::duration_cast<Duration>(m_clock.now() - m_lastCycle);
   return static_cast<T>(elapsed);
}
