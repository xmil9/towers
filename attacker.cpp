//
// Nov-2020, Michael Lindner
// MIT license
//
#include "attacker.h"
#include "glm/gtc/matrix_transform.hpp"
#include "essentutils/fputil.h"


Attacker::Attacker(Sprite sp, int hp, const Path& path, const MapCoordSys& cs)
: m_sprite{std::move(sp)}, m_hp{hp}, m_pos{path.start().center()},
  m_currTurn{0}, m_path{path}, m_coordSys{cs}
{
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

   assert(m_pos && m_currTurn);

   Rect nextTurn = m_path[*m_currTurn + 1];
   const Pos nextTurnPos = nextTurn.center();
   const glm::vec2 dist = nextTurnPos - *m_pos;
   const glm::vec2 direction = glm::normalize(dist);

   glm::vec2 offset = m_speed * direction;
   // Limit movement to next turn.
   if (sutil::greaterEqual(glm::length(offset), glm::length(dist)))
   {
      offset = dist;
      ++*m_currTurn;
   }

   setPosition(*m_pos + offset);
}


bool Attacker::isAtLastPosition() const
{
   if (!m_pos)
      return true;
   return isEqualPos(*m_pos, m_path.finish().center());
}


void Attacker::setPosition(std::optional<Pos> pos)
{
   m_pos = pos;
   if (pos)
      m_sprite.setPosition(m_coordSys.toRenderCoords(*m_pos));
   else
      ; // todo - set flag to remove attacker
}
