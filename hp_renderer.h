//
// Jan-2021, Michael Lindner
// MIT license
//
#pragma once
#include "sprite.h"

class Renderer2;


class HpRenderer
{
public:
   void setup(Sprite&& hpMeter, Sprite&& hpStatus);
   void render(Renderer2& renderer, PixPos at);

private:
   Sprite m_hpMeter;
   Sprite m_hpStatus;
};
