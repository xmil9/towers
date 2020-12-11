//
// Dec-2020, Michael Lindner
// MIT license
//
#include "defender.h"
#include "essentutils/fputil.h"
#include "glm/gtx/vector_angle.hpp"


static constexpr NormVec Up{0., -1.};


Defender::Defender(Sprite sp, NormVec size, NormPos center, NormCoord range, int damage,
                   const MapCoordSys& cs, std::vector<Attacker>& attackers)
: m_range{range}, m_damage{damage}, m_size{size}, m_center{center},
  m_coordSys{cs}, m_sprite{std::move(sp)}, m_attackers{attackers}
{
   setSize(size);
   setPosition(center);
}


void Defender::render(const gll::Program& shaders)
{
   if (m_target)
      calcRotation();
   m_sprite.render(shaders);
}


void Defender::update()
{
   findTarget();
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


void Defender::findTarget()
{
   m_target.reset();

   for (auto& attacker : m_attackers)
   {
      if (isInRange(attacker))
         m_target = &attacker;
   }
}


bool Defender::isInRange(const Attacker& attacker) const
{
   const auto pos = attacker.position();
   if (!pos)
      return false;

   const NormCoord dist = glm::length(m_center - *pos);
   return sutil::lessEqual(dist, m_range);
}


void Defender::calcRotation()
{
   const auto targetDir = targetDirection();
   if (!targetDir)
      return;

   const Angle_t rot{-glm::orientedAngle(glm::normalize(*targetDir), Up)};
   m_sprite.setRotation(rot);
}


std::optional<NormVec> Defender::targetDirection() const
{
   if (!m_target)
      return std::nullopt;

   const auto targetPos = (*m_target)->position();
   if (!targetPos)
      return std::nullopt;

   return *targetPos - m_center;
}
