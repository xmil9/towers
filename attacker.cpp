//
// Nov-2020, Michael Lindner
// MIT license
//
#include "attacker.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/vector_angle.hpp"
#include "essentutils/fputil.h"


static constexpr NormVec Up{0., -1.};


Attacker::Attacker(AttackerLook look, NormCoord size, const Attribs& attribs,
                   const OffsetPath& path, const MapCoordSys* cs)
: m_look{std::move(look)}, m_initialAttribs{attribs}, m_currAttribs{attribs},
  m_center{path.start().center()}, m_currTurn{0}, m_path{path}, m_coordSys{cs}
{
   assert(m_coordSys);

   setSize(m_coordSys->makeEquivalentMapSize(size, m_look.size()));
   setPosition(path.start().center());
}


void Attacker::render(const gll::Program& shaders)
{
   if (!hasStarted())
      return;

   const PixPos center = m_coordSys->toRenderCoords(*m_center);

   if (isAlive())
   {
      calcRotation();
      m_look.render(shaders, center);
   }
   else
   {
      m_look.renderExploded(shaders, center);
   }
}


void Attacker::update()
{
   if (m_currAttribs.launchDelay == 0)
      move();
   else
      --m_currAttribs.launchDelay;
}


void Attacker::hit(int damage)
{
   if (!isAlive())
      return;

   m_currAttribs.hp = std::max(0, m_currAttribs.hp - damage);
}


bool Attacker::canBeRemoved() const
{
   const bool isAtGoal = !m_center;
   const bool hasExploded = m_look.hasExplosionFinished();
   return isAtGoal || hasExploded;
}


void Attacker::move()
{
   if (!isAlive())
      return;

   // Cannot move further.
   if (isAtLastPosition())
   {
      setPosition(std::nullopt);
      m_currTurn = std::nullopt;
      return;
   }

   assert(m_center && m_currTurn);

   const NormVec dir = direction();
   NormVec offset = m_currAttribs.speed * glm::normalize(dir);
   // Limit movement to next turn.
   if (sutil::greaterEqual(glm::length(offset), glm::length(dir)))
   {
      offset = dir;
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
}


void Attacker::setSize(NormVec size)
{
   m_look.setSize(m_coordSys->toRenderCoords(size));
}


void Attacker::calcRotation()
{
   const Angle_t rot{glm::angle(normedDirection(), Up)};
   m_look.setRotation(rot);
}


NormVec Attacker::direction() const
{
   if (!m_center || !m_currTurn)
      return Up;

   NormPos pos = *m_center;
   NormPos nextPos = pos;
   if (*m_currTurn < m_path.size() - 1)
   {
      nextPos = m_path[*m_currTurn + 1].center();
   }
   else
   {
      pos = m_path[*m_currTurn - 1].center();
      nextPos = m_path[*m_currTurn].center();
   }

   return nextPos - pos;
}
