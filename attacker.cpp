//
// Nov-2020, Michael Lindner
// MIT license
//
#include "attacker.h"
#include "glm/gtc/matrix_transform.hpp"
#include "essentutils/fputil.h"


Attacker::Attacker(Sprite sp, NormVec size, int hp, float speed, const OffsetPath& path,
                   const MapCoordSys& cs)
: m_size{size}, m_hp{hp}, m_speed{speed}, m_center{path.start().center()},
  m_currTurn{0}, m_path{path}, m_coordSys{cs}, m_sprite{std::move(sp)}
{
   setSize(size);
   setPosition(path.start().center());
}


void Attacker::update()
{
   move();
}


void Attacker::move()
{
   // Cannot move anymore.
   if (isAtLastPosition())
   {
      setPosition(std::nullopt);
      m_currTurn = std::nullopt;
      return;
   }

   assert(m_center && m_currTurn);

   const NormRect nextTurn = m_path[*m_currTurn + 1];
   const NormPos nextTurnPos = nextTurn.center();
   const NormVec dist = nextTurnPos - *m_center;
   const NormVec direction = glm::normalize(dist);

   NormVec offset = m_speed * direction;
   // Limit movement to next turn.
   if (sutil::greaterEqual(glm::length(offset), glm::length(dist)))
   {
      offset = dist;
      ++*m_currTurn;
   }

   setPosition(*m_center + offset);
}


bool Attacker::isAtLastPosition() const
{
   if (!m_center)
      return true;
   return isEqual(*m_center, m_path.finish().center());
}


void Attacker::setPosition(std::optional<NormPos> center)
{
   m_center = center;
   if (m_center)
   {
      // Sprite uses left-top coord as position.
      m_sprite.setPosition(m_coordSys.toRenderCoords(*m_center - m_size / 2.f));
   }
   else
   {
      ; // todo - set flag to remove attacker
   }
}


void Attacker::setSize(NormVec size)
{
   m_size = size;
   m_sprite.setSize(m_coordSys.toRenderCoords(size));
}
