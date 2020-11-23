//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "dim.h"
#include "sprite.h"
#include <cstddef>


class Attacker
{
public:
   Attacker(Sprite sp, int hp);

private:
   Dim2<std::size_t> m_pos;
   float m_speed = 0.f;
   int m_hp = 0;
   Sprite m_sprite;
};


inline Attacker::Attacker(Sprite sp, int hp)
   : m_sprite{sp}, m_hp{hp}
{
}
