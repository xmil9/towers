//
// Nov-2020, Michael Lindner
// MIT license
//
#include "attacker.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/vector_angle.hpp"
#include "essentutils/fputil.h"


static constexpr NormVec Up{0., -1.};


Attacker::Attacker(Sprite sp, NormVec size, int hp, float speed, const OffsetPath& path,
                   Animation explosion,
                   std::vector<Animation>* activeExplosions,
                   const MapCoordSys* cs)
: m_size{size}, m_hp{hp}, m_speed{speed}, m_center{path.start().center()},
  m_currTurn{0}, m_path{path}, m_coordSys{cs}, m_sprite{std::move(sp)},
  m_explosion{explosion}, m_activeExplosions{activeExplosions}
{
   setSize(size);
   setPosition(path.start().center());
}


void Attacker::render(const gll::Program& shaders)
{
   calcRotation();
   m_sprite.render(shaders);
}


void Attacker::update()
{
   move();
}


void Attacker::damage(int amount)
{
   m_hp = std::max(0, m_hp - amount);

   if (!isAlive())
      m_activeExplosions->push_back(m_explosion);
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

   const NormVec dir = direction();
   NormVec offset = m_speed * glm::normalize(dir);
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
   if (m_center)
   {
      // Sprite uses left-top coord as position.
      m_sprite.setPosition(m_coordSys->toRenderCoords(*m_center - m_size / 2.f));
   }
   else
   {
      ; // todo - set flag to remove attacker
   }
}


void Attacker::setSize(NormVec size)
{
   m_size = size;
   m_sprite.setSize(m_coordSys->toRenderCoords(size));
}


void Attacker::calcRotation()
{
   const Angle_t rot{glm::angle(normedDirection(), Up)};
   m_sprite.setRotation(rot);
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
