//
// essentutils
// Type traits utilities.
//
// Jul-2020, Michael Lindner
// MIT license
//
#pragma once
#include <iterator>
#include <type_traits>


namespace sutil
{
///////////////////

template <typename T, typename = void> inline constexpr bool isIterator_v = false;

// Will only pass SFINAE, if the given type defines iterator_category.
template <typename T>
inline constexpr bool
   isIterator_v<T, std::void_t<typename std::iterator_traits<T>::iterator_category>> =
      true;

template <typename T> struct IsIterator : std::bool_constant<isIterator_v<T>>
{
};


///////////////////

template <typename T, typename... Types> struct IsOneOf : std::false_type
{
};

template <typename T, typename U> struct IsOneOf<T, U> : std::is_same<T, U>
{
};

template <typename T, typename U, typename... Rest>
struct IsOneOf<T, U, Rest...>
: std::integral_constant<bool, IsOneOf<T, U>::value || IsOneOf<T, Rest...>::value>
{
};

template <typename T, typename... Types>
inline constexpr bool isOneOf_v = IsOneOf<T, Types...>::value;

} // namespace sutil
