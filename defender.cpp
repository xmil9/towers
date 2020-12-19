//
// Dec-2020, Michael Lindner
// MIT license
//
#include "defender.h"
#include "essentutils/fputil.h"
#include "glm/gtx/vector_angle.hpp"


static constexpr NormVec Up{0., -1.};


Defender::Defender(DefenderLook look, NormVec size, NormPos center, NormCoord range,
                   int damage, const MapCoordSys* cs, std::vector<Attacker>& attackers)
: m_look{std::move(look)}, m_range{range}, m_damage{damage}, m_center{center},
  m_coordSys{cs}, m_attackers{attackers}
{
   assert(m_coordSys);

   setSize(size);
   setPosition(center);
}


void Defender::render(const gll::Program& shaders)
{
   const PixPos center = m_coordSys->toRenderCoords(m_center);

   if (m_target)
   {
      calcRotation();
      m_look.renderFiring(shaders, center);
   }
   else
   {
      m_look.render(shaders, center);
   }
}


void Defender::update()
{
   if (findTarget())
      shoot();
}


void Defender::setPosition(NormPos center)
{
   m_center = center;
}


void Defender::setSize(NormVec size)
{
   m_look.setSize(m_coordSys->toRenderCoords(size));
}


bool Defender::findTarget()
{
   m_target.reset();

   for (auto& attacker : m_attackers)
   {
      if (attacker.isAlive() && isInRange(attacker))
      {
         m_target = &attacker;
         break;
      }
   }

   return !!m_target;
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
   m_look.setRotation(rot);
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


void Defender::shoot()
{
   if (m_target)
      (*m_target)->hit(m_damage);
}
