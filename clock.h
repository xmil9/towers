//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include <chrono>


template <typename T, typename Duration = std::chrono::milliseconds> class Clock
{
 public:
   using value_type = T;
   using duration_type = Duration;

 public:
   T measureCycle();
   T cycleLength() const;
   T elapsed() const;

 private:
   using TimePoint = std::chrono::steady_clock::time_point;

 private:
   std::chrono::steady_clock m_clock;
   TimePoint m_lastCycle;
   Duration m_cycleLength;
};


template <typename T, typename Duration> T Clock<T, Duration>::measureCycle()
{
   const TimePoint now = m_clock.now();
   m_cycleLength = std::chrono::duration_cast<Duration>(now - m_lastCycle);
   m_lastCycle = now;
   return cycleLength();
}


template <typename T, typename Duration> T Clock<T, Duration>::cycleLength() const
{
   return static_cast<T>(m_cycleLength.count());
}


template <typename T, typename Duration> T Clock<T, Duration>::elapsed() const
{
   const auto elapsed = std::chrono::duration_cast<Duration>(m_clock.now() - m_lastCycle);
   return static_cast<T>(elapsed.count());
}
