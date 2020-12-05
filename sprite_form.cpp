//
// Dec-2020, Michael Lindner
// MIT license
//
#include "sprite_form.h"


PixDim scaleTo(PixDim source, PixCoord length)
{
   PixCoord& longer = source.x >= source.y ? source.x : source.y;
   PixCoord& shorter = source.x >= source.y ? source.y : source.x;
   const float aspect = shorter / longer;
   longer = length;
   shorter = aspect * longer;
   return source;
}
