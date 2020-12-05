//
// Dec-2020, Michael Lindner
// MIT license
//
#pragma once


///////////////////

template<typename Pos>
struct Rect
{
   using Coord = typename Pos::value_type;
   using valye_type = Coord;

   Rect() = default;
   Rect(Coord left, Coord top, Coord right, Coord bottom);
   Rect(Pos lt, Pos rb);

   Coord l = Coord(0);
   Coord t = Coord(0);
   Coord r = Coord(0);
   Coord b = Coord(0);

   bool contains(Pos at) const;
   Pos center() const;
};


template<typename Pos>
Rect<Pos>::Rect(Coord left, Coord top, Coord right, Coord bottom)
   : l{left}, t{top}, r{right}, b{bottom}
{
}

template<typename Pos>
Rect<Pos>::Rect(Pos lt, Pos rb)
   : Rect(lt.x, lt.y, rb.x, rb.y)
{
}

template<typename Pos>
bool Rect<Pos>::contains(Pos at) const
{
   return l <= at.x && at.x < r && t <= at.y && at.y < b;
}

template<typename Pos>
inline Pos Rect<Pos>::center() const
{
   return {(l + r) / Coord(2), (t + b) / Coord(2)};
}
