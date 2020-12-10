//
// Dec-2020, Michael Lindner
// MIT license
//
#include "defender.h"




Defender::Defender(Sprite sp, NormVec size, NormPos center, const MapCoordSys& cs)
: m_size{size}, m_center{center}, m_coordSys{cs}, m_sprite{std::move(sp)}
{
   setSize(size);
   setPosition(center);
}


void Defender::render(const gll::Program& shaders)
{
   m_sprite.render(shaders);
}


void Defender::update()
{
}


void Defender::setPosition(NormPos center)
{
   m_center = center;
   // Sprite uses left-top coord as position.
   m_sprite.setPosition(m_coordSys.toRenderCoords(m_center - m_size / 2.f));
}


void Defender::setSize(NormVec size)
{
   m_size = size;
   m_sprite.setSize(m_coordSys.toRenderCoords(size));
}
