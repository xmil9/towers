//
// essentutils
// Floating point utilities.
//
// Jun-2020, Michael Lindner
// MIT license
//
#pragma once
#include <cassert>
#include <cmath>
#include <type_traits>


namespace esl
{

////////////////////

template <typename FP> struct FpTraits
{
};


template <> struct FpTraits<double>
{
   // Threshold below which two fp values are considered equal.
   static constexpr double epsilon = 0.0000001;
   static double sqrt(double val) { return std::sqrt(val); }
};


template <> struct FpTraits<float>
{
   static constexpr float epsilon = 0.00001f;
   static float sqrt(float val) { return std::sqrtf(val); }
};


template <> struct FpTraits<long double>
{
   static constexpr long double epsilon = 0.000000001L;
   static long double sqrt(long double val) { return std::sqrtl(val); }
};


///////////////////

// Selects a floating point type based on a given type (the given type could be any type,
// not just floating point types).
template <typename T, typename FP = double>
using FpType = std::conditional_t<
   std::is_same_v<T, float>, float,
   std::conditional_t<
      std::is_same_v<T, double>, double,
      std::conditional_t<std::is_same_v<T, long double>, long double, FP>>>;


///////////////////

// Comparision functions for floating point data types.

template <typename FP> bool fpEqual(FP a, FP b, FP eps)
{
   static_assert(std::is_floating_point_v<FP>);
   assert(eps >= 0.0);
   return std::abs(a - b) <= eps;
}


template <typename FP, typename Traits = FpTraits<FP>> bool fpEqual(FP a, FP b)
{
   return fpEqual(a, b, Traits::epsilon);
}


template <typename FP> bool fpLess(FP a, FP b, FP eps)
{
   static_assert(std::is_floating_point_v<FP>);
   assert(eps >= 0.0);
   // Check that a is smaller than b by at least the epsilon value
   // because within that threshold they would still be considered
   // equal.
   return a - b < -eps;
}


template <typename FP, typename Traits = FpTraits<FP>> bool fpLess(FP a, FP b)
{
   return fpLess(a, b, Traits::epsilon);
}


template <typename FP> bool fpLessEqual(FP a, FP b, FP eps)
{
   static_assert(std::is_floating_point_v<FP>);
   assert(eps >= 0.0);
   // Check that b is larger than a by at least the epsilon value
   // because within that threshold they would still be considered
   // equal.
   return a - b <= eps;
}


template <typename FP, typename Traits = FpTraits<FP>> bool fpLessEqual(FP a, FP b)
{
   return fpLessEqual(a, b, Traits::epsilon);
}


template <typename FP> bool fpGreater(FP a, FP b, FP eps)
{
   static_assert(std::is_floating_point_v<FP>);
   assert(eps >= 0.0);
   // Check that a is larger than b by at least the epsilon value
   // because within that threshold they would still be considered
   // equal.
   return a - b > eps;
}


template <typename FP, typename Traits = FpTraits<FP>> bool fpGreater(FP a, FP b)
{
   return fpGreater(a, b, Traits::epsilon);
}


template <typename FP> bool fpGreaterEqual(FP a, FP b, FP eps)
{
   static_assert(std::is_floating_point_v<FP>);
   assert(eps >= 0.0);
   // Check that b is smaller than a by at least the epsilon value
   // because within that threshold they would still be considered
   // equal.
   return a - b >= -eps;
}


template <typename FP, typename Traits = FpTraits<FP>> bool fpGreaterEqual(FP a, FP b)
{
   return fpGreaterEqual(a, b, Traits::epsilon);
}


///////////////////

// Convenience functions that use a comparison method that is appropriate for
// the used data type.

template <typename T> bool equal(const T& a, const T& b)
{
   if constexpr (std::is_floating_point_v<T>)
      return fpEqual(a, b);
   else
      return a == b;
}


template <typename T> bool less(const T& a, const T& b)
{
   if constexpr (std::is_floating_point_v<T>)
      return fpLess(a, b);
   else
      return a < b;
}


template <typename T> bool lessEqual(const T& a, const T& b)
{
   if constexpr (std::is_floating_point_v<T>)
      return fpLessEqual(a, b);
   else
      return a <= b;
}


template <typename T> bool greater(const T& a, const T& b)
{
   if constexpr (std::is_floating_point_v<T>)
      return fpGreater(a, b);
   else
      return a > b;
}


template <typename T> bool greaterEqual(const T& a, const T& b)
{
   if constexpr (std::is_floating_point_v<T>)
      return fpGreaterEqual(a, b);
   else
      return a >= b;
}


// Convenience function to call the appropriate sqrt version for a given
// value type. Floating point types will use their respective traits,
// other type can specify the version explicitly, e.g. sqrt<float>(5).
template<typename T>
T sqrt(T val)
{
   return FpTraits<T>::sqrt(val);
}

} // namespace esl
