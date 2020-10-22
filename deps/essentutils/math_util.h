//
// essentutils
// Math utilities.
//
// Jun-2020, Michael Lindner
// MIT license
//
#pragma once
#include <cassert>
#include <type_traits>
#include <utility>
#include "fputil.h"


namespace sutil
{

////////////////////
// Value ranges.

// Limits a value to given min and max values.
template <typename T> T clampToRange(T val, T min, T max)
{
   // Normalize.
   if (min > max)
   {
      using std::swap;
      swap(min, max);
   }

   // Clamp.
   if (val < min)
      val = min;
   if (val > max)
      val = max;

   assert(min <= val && val <= max);
   return val;
}


// Shifts a given value into the range of given min and max values. The
// value will end up in the same position within the range.
// Example:
//   780 shift into [0, 360] => 60
template <typename T> T shiftIntoRange(T val, T min, T max)
{
   static_assert(std::is_integral_v<T>);

   // Normalize.
   if (min > max)
   {
      using std::swap;
      swap(min, max);
   }

   // Guard against div by zero.
   T len = max - min;
   if (len == 0)
      return val;

   // Shift.
   if (val < min)
      val = max - (min - val) % len;
   if (val > max)
      val = min + (val - min) % len;

   assert(min <= val && val <= max);
   return val;
}


////////////////////
// Angle unit conversions.

template <typename FP> constexpr FP Pi = FP{3.1415926535};


template <typename FP> constexpr FP degreesFromRadians(FP rad)
{
   static_assert(std::is_floating_point_v<FP>);
   return rad * FP{180.0} / Pi<FP>;
}


template <typename FP> constexpr FP radiansFromDegrees(FP deg)
{
   static_assert(std::is_floating_point_v<FP>);
   return deg * Pi<FP> / FP{180.0};
}


////////////////////
// Sign

enum class Sign
{
   Pos,
   Neg,
   None
};


template <typename T>
Sign sign(T val)
{
   if (val > T{0})
      return Sign::Pos;
   else if (val < T{0})
      return Sign::Neg;
   return Sign::None;
}


inline Sign sign(double val)
{
   if (fpGreater(val, 0.0))
      return Sign::Pos;
   else if (fpLess(val, 0.0))
      return Sign::Neg;
   return Sign::None;
}


inline Sign sign(float val)
{
   if (fpGreater(val, 0.0f))
      return Sign::Pos;
   else if (fpLess(val, 0.0f))
      return Sign::Neg;
   return Sign::None;
}


inline Sign sign(long double val)
{
   if (fpGreater(val, 0.0L))
      return Sign::Pos;
   else if (fpLess(val, 0.0L))
      return Sign::Neg;
   return Sign::None;
}

} // namespace sutil
