//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once


// POD data structure for a point.
// This will be used for arrays of elements of type T, so the memory layout
// must not contain anything else but Ts.
template<typename T>
struct Point2
{
   using value_type = T;

   T x;
   T y;
};
