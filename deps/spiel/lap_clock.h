//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include <chrono>


namespace sp
{
///////////////////

// Clock to measure repeated events.
template <typename T, typename Unit = std::chrono::milliseconds> class LapClock
{
 public:
   using TimePoint = std::chrono::steady_clock::time_point;
   using UnitType = Unit;
   using ValueType = T;

 public:
   // Starts a new lap.
   TimePoint nextLap();
   // Returns the length of the last full lap.
   T lapLength(float factor = 1.0f) const;
   // Returns the length of time passed since the current lap started.
   T elapsedInLap(float factor = 1.0f) const;

 private:
   std::chrono::steady_clock m_clock;
   TimePoint m_lastLap;
   Unit m_lapLength{0};
};


template <typename T, typename Duration>
typename LapClock<T, Duration>::TimePoint LapClock<T, Duration>::nextLap()
{
   const TimePoint now = m_clock.now();
   m_lapLength = std::chrono::duration_cast<Duration>(now - m_lastLap);
   m_lastLap = now;
   return now;
}


template <typename T, typename Duration>
T LapClock<T, Duration>::lapLength(float factor) const
{
   return static_cast<T>(m_lapLength.count() * factor);
}


template <typename T, typename Duration>
T LapClock<T, Duration>::elapsedInLap(float factor) const
{
   const auto elapsed = std::chrono::duration_cast<Duration>(m_clock.now() - m_lastLap);
   return static_cast<T>(elapsed.count() * factor);
}

} // namespace sp
