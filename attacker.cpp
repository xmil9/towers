//
// Nov-2020, Michael Lindner
// MIT license
//
#include "attacker.h"
#include "glm/gtc/matrix_transform.hpp"


void Attacker::update()
{
   move();
}


void Attacker::move()
{
   // Cannot move anymore.
   if (!m_pos)
      return;

   // Current position is the last one.
   const auto nextTurn = m_path.nextTurn(*m_pos);
   if (!nextTurn)
   {
      m_pos = std::nullopt;
      return;
   }

   const Pos nextTurnPos = nextTurn->center();
   const glm::vec2 dist = *m_pos - nextTurnPos;
   const glm::vec2 direction = glm::normalize(dist);
   
   glm::vec2 offset = m_speed * direction;
   // Limit offset to next turn.
   if (offset.length() > dist.length())
      offset = dist;
   
   m_pos = *m_pos + offset;
}
