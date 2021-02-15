//
// Feb-2021, Michael Lindner
// MIT license
//
#pragma once


template<typename T>
constexpr T UiScale(T val)
{
   return val * T(1);//T(2) / T(3);
}
