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


namespace esl
{
///////////////////

template <typename Fp> class Angle
{
 public:
   using value_type = Fp;

   Angle() = default;
   explicit constexpr Angle(Fp rad);
   template <typename T> static constexpr Angle fromRadians(T rad);
   template <typename T> static constexpr Angle fromDegrees(T deg);
   Angle(const Angle&) = default;
   Angle(Angle&&) = default;

   Angle& operator=(const Angle&) = default;
   Angle& operator=(Angle&&) = default;
   constexpr Angle& operator=(Fp rad);

   constexpr operator Fp() const { return radians(); }
   constexpr Fp radians() const { return m_rad; }
   constexpr Fp degrees() const { return esl::degrees(m_rad); }

   constexpr Angle& operator+=(Angle a);
   constexpr Angle& operator+=(Fp rad);
   constexpr Angle& operator-=(Angle a);
   constexpr Angle& operator-=(Fp rad);
   constexpr Angle& operator*=(Angle a);
   constexpr Angle& operator*=(Fp rad);
   constexpr Angle& operator/=(Angle a);
   constexpr Angle& operator/=(Fp rad);

   friend constexpr void swap(Angle<Fp>& a, Angle<Fp>& b) noexcept
   {
      std::swap(a.m_rad, b.m_rad);
   }

 private:
   // Store in radians.
   Fp m_rad = Fp(0);
};


template <typename Fp> constexpr Angle<Fp>::Angle(Fp rad) : m_rad(rad)
{
   static_assert(std::is_floating_point_v<Fp>);
}

template <typename Fp>
template <typename T>
constexpr Angle<Fp> Angle<Fp>::fromRadians(T rad)
{
   return Angle<Fp>(rad);
}

template <typename Fp>
template <typename T>
constexpr Angle<Fp> Angle<Fp>::fromDegrees(T deg)
{
   return Angle<Fp>{esl::radians(deg)};
}

template <typename Fp> constexpr Angle<Fp>& Angle<Fp>::operator=(Fp rad)
{
   m_rad = rad;
   return *this;
}

template <typename Fp> constexpr Angle<Fp>& Angle<Fp>::operator+=(Angle a)
{
   return *this += a.m_rad;
}

template <typename Fp> constexpr Angle<Fp>& Angle<Fp>::operator+=(Fp rad)
{
   m_rad += rad;
   return *this;
}

template <typename Fp> constexpr Angle<Fp>& Angle<Fp>::operator-=(Angle a)
{
   return *this -= a.m_rad;
}

template <typename Fp> constexpr Angle<Fp>& Angle<Fp>::operator-=(Fp rad)
{
   m_rad -= rad;
   return *this;
}

template <typename Fp> constexpr Angle<Fp>& Angle<Fp>::operator*=(Angle a)
{
   return *this *= a.m_rad;
}

template <typename Fp> constexpr Angle<Fp>& Angle<Fp>::operator*=(Fp rad)
{
   m_rad *= rad;
   return *this;
}

template <typename Fp> constexpr Angle<Fp>& Angle<Fp>::operator/=(Angle a)
{
   return *this /= a.m_rad;
}

template <typename Fp> constexpr Angle<Fp>& Angle<Fp>::operator/=(Fp rad)
{
   // No protection again div by zero.
   assert(rad != 0.0);
   m_rad /= rad;
   return *this;
}


///////////////////

// Free function comparisions between two angles or an angle and a scalar.
// Note that the scalar overloads are necessary otherwise the compiler will implicitly
// convert the angle operand of a mixed scalar-angle operation to a scalar which will call
// the comparision operator of the scalar which in turn will not perform a correct
// floating point comparision.

// Equality.
template <typename T, typename U>
constexpr bool operator==(const Angle<T>& a, const Angle<U>& b)
{
   using Common = std::common_type_t<T, U>;
   return esl::equal<Common>(a, b);
}

template <typename T, typename Scalar>
constexpr bool operator==(const Angle<T>& a, Scalar b)
{
   using Common = std::common_type_t<T, Scalar>;
   return esl::equal<Common>(a, b);
}

template <typename Scalar, typename T>
constexpr bool operator==(Scalar a, const Angle<T>& b)
{
   using Common = std::common_type_t<Scalar, T>;
   return esl::equal<Common>(a, b);
}

// Inquality.
template <typename T, typename U>
constexpr bool operator!=(const Angle<T>& a, const Angle<U>& b)
{
   return !(a == b);
}

template <typename T, typename Scalar>
constexpr bool operator!=(const Angle<T>& a, Scalar b)
{
   return !(a == b);
}

template <typename Scalar, typename T>
constexpr bool operator!=(Scalar a, const Angle<T>& b)
{
   return !(a == b);
}

// Less-than.
template <typename T, typename U>
constexpr bool operator<(const Angle<T>& a, const Angle<U>& b)
{
   using Common = std::common_type_t<T, U>;
   return esl::less<Common>(a, b);
}

template <typename T, typename Scalar>
constexpr bool operator<(const Angle<T>& a, Scalar b)
{
   using Common = std::common_type_t<T, Scalar>;
   return esl::less<Common>(a, b);
}

template <typename Scalar, typename T>
constexpr bool operator<(Scalar a, const Angle<T>& b)
{
   using Common = std::common_type_t<Scalar, T>;
   return esl::less<Common>(a, b);
}

// Less-or-equal-than.
template <typename T, typename U>
constexpr bool operator<=(const Angle<T>& a, const Angle<U>& b)
{
   using Common = std::common_type_t<T, U>;
   return esl::lessEqual<Common>(a, b);
}

template <typename T, typename Scalar>
constexpr bool operator<=(const Angle<T>& a, Scalar b)
{
   using Common = std::common_type_t<T, Scalar>;
   return esl::lessEqual<Common>(a, b);
}

template <typename Scalar, typename T>
constexpr bool operator<=(Scalar a, const Angle<T>& b)
{
   using Common = std::common_type_t<Scalar, T>;
   return esl::lessEqual<Common>(a, b);
}

// Greater-than.
template <typename T, typename U>
constexpr bool operator>(const Angle<T>& a, const Angle<U>& b)
{
   return !(a <= b);
}

template <typename T, typename Scalar>
constexpr bool operator>(const Angle<T>& a, Scalar b)
{
   return !(a <= b);
}

template <typename Scalar, typename T>
constexpr bool operator>(Scalar a, const Angle<T>& b)
{
   return !(a <= b);
}

// Greater-or-equal-than.
template <typename T, typename U>
constexpr bool operator>=(const Angle<T>& a, const Angle<U>& b)
{
   return !(a < b);
}

template <typename T, typename Scalar>
constexpr bool operator>=(const Angle<T>& a, Scalar b)
{
   return !(a < b);
}

template <typename Scalar, typename T>
constexpr bool operator>=(Scalar a, const Angle<T>& b)
{
   return !(a < b);
}


///////////////////

// Free function arithmatic between two angles or an angle and a scalar.
// Note that the scalar overloads are necessary because the compiler will not implicitly
// convert a scalar to and angle.
// Don't define overloads with a scalar as the first operand because it is not clear if
// the result should be a scalar or an angle.

template <typename T, typename U>
constexpr Angle<std::common_type_t<T, U>> operator+(Angle<T> a, Angle<U> b)
{
   return a += b;
}

template <typename T, typename Scalar>
constexpr Angle<std::common_type_t<T, Scalar>> operator+(Angle<T> a, Scalar b)
{
   using Common = std::common_type_t<T, Scalar>;
   return a += static_cast<Common>(b);
}

template <typename T, typename U>
constexpr Angle<std::common_type_t<T, U>> operator-(Angle<T> a, Angle<U> b)
{
   return a -= b;
}

template <typename T, typename Scalar>
constexpr Angle<std::common_type_t<T, Scalar>> operator-(Angle<T> a, Scalar b)
{
   using Common = std::common_type_t<T, Scalar>;
   return a -= static_cast<Common>(b);
}

template <typename T, typename U>
constexpr Angle<std::common_type_t<T, U>> operator*(Angle<T> a, Angle<U> b)
{
   return a *= b;
}

template <typename T, typename Scalar>
constexpr Angle<std::common_type_t<T, Scalar>> operator*(Angle<T> a, Scalar b)
{
   using Common = std::common_type_t<T, Scalar>;
   return a *= static_cast<Common>(b);
}

template <typename T, typename U>
constexpr Angle<std::common_type_t<T, U>> operator/(Angle<T> a, Angle<U> b)
{
   return a /= b;
}

template <typename T, typename Scalar>
constexpr Angle<std::common_type_t<T, Scalar>> operator/(Angle<T> a, Scalar b)
{
   using Common = std::common_type_t<T, Scalar>;
   return a /= static_cast<Common>(b);
}

} // namespace esl


///////////////////
// Hashing

namespace std
{
template <typename Fp> struct hash<esl::Angle<Fp>>
{
   std::size_t operator()(const esl::Angle<Fp>& a) const noexcept
   {
      return std::hash<Fp>{}(a.operator Fp());
   }
};
} // namespace std
