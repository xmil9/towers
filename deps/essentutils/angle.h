//
// essentutils
// Angle class.
//
// Oct-2020, Michael Lindner
// MIT license
//
#pragma once
#include "fputil.h"
#include "math_util.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <type_traits>


namespace sutil
{
///////////////////

template <typename Fp> class Angle
{
 public:
   Angle() = default;
   explicit constexpr Angle(Fp rad);
   template <typename T> static constexpr Angle makeRadians(T rad);
   template <typename T> static constexpr Angle makeDegrees(T deg);
   Angle(const Angle&) = default;
   Angle(Angle&&) = default;

   Angle& operator=(const Angle&) = default;
   Angle& operator=(Angle&&) = default;

   constexpr operator Fp() const { return radians(); }
   constexpr Fp radians() const { return m_rad; }
   constexpr Fp degrees() const { return sutil::degreesFromRadians(m_rad); }

   friend void swap(Angle<Fp>& a, Angle<Fp>& b) noexcept
   {
      std::swap(a.m_rad, b.m_rad);
   }

 private:
   // Store in radians.
   Fp m_rad = Fp(0);
};


template <typename Fp>
constexpr Angle<Fp>::Angle(Fp rad) : m_rad(rad)
{
   static_assert(std::is_floating_point_v<Fp>);
}

template <typename Fp>
template <typename T>
constexpr Angle<Fp> Angle<Fp>::makeRadians(T rad)
{
   return Angle<Fp>(rad);
}

template <typename Fp>
template <typename T>
constexpr Angle<Fp> Angle<Fp>::makeDegrees(T deg)
{
   return Angle<Fp>{sutil::radiansFromDegrees(deg)};
}


///////////////////

template <typename T, typename U> bool operator==(const Angle<T>& a, const Angle<U>& b)
{
   using Common = std::common_type_t<T, U>;
   return sutil::equal<Common>(a, b);
}

template <typename T, typename U> bool operator!=(const Angle<T>& a, const Angle<U>& b)
{
   return !(a == b);
}

template <typename T, typename U> bool operator<(const Angle<T>& a, const Angle<U>& b)
{
   using Common = std::common_type_t<T, U>;
   return sutil::less<Common>(a, b);
}

template <typename T, typename U> bool operator<=(const Angle<T>& a, const Angle<U>& b)
{
   using Common = std::common_type_t<T, U>;
   return sutil::lessEqual<Common>(a, b);
}

template <typename T, typename U> bool operator>(const Angle<T>& a, const Angle<U>& b)
{
   return !(a <= b);
}

template <typename T, typename U> bool operator>=(const Angle<T>& a, const Angle<U>& b)
{
   return !(a < b);
}

} // namespace sutil


namespace std
{
template <typename Fp> struct hash<sutil::Angle<Fp>>
{
   std::size_t operator()(const sutil::Angle<Fp>& a) const noexcept
   {
      return std::hash<Fp>{}(a.operator Fp());
   }
};
} // namespace std
