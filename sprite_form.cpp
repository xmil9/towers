//
// Dec-2020, Michael Lindner
// MIT license
//
#include "sprite_form.h"


RenderDim scaleTo(float length, RenderDim source)
{
   float& longer = source.x >= source.y ? source.x : source.y;
   float& shorter = source.x >= source.y ? source.y : source.x;
   const float aspect = shorter / longer;
   longer = length;
   shorter = aspect * longer;
   return source;
}
