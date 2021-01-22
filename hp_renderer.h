//
// Jan-2021, Michael Lindner
// MIT license
//
#pragma once
#include "coords.h"
#include "sprite.h"

class Renderer2;


class HpRenderer
{
public:
   void setup(Sprite&& status, PixVec offset);
   void render(Renderer2& renderer, PixPos atSpriteCenter, float ratio);

private:
   Sprite m_status;
   PixVec m_offset{0.f, 0.f};
};
