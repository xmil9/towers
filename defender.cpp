//
// Dec-2020, Michael Lindner
// MIT license
//
#include "defender.h"
#include "essentutils/fputil.h"
#include "glm/gtx/vector_angle.hpp"


static constexpr MapVec Up{0.f, -1.f};


Defender::Defender(DefenderLook look, MapCoord size, MapPos center,
                   const Attribs& attribs, const MapCoordSys* cs,
                   std::vector<Attacker>* attackers)
: m_look{std::move(look)}, m_attribs{attribs}, m_center{center}, m_coordSys{cs},
  m_attackers{attackers}
{
   assert(m_coordSys);
   assert(m_attackers);

   setSize(m_coordSys->scaleToSize(size, m_look.size()));
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


void Defender::setPosition(MapPos center)
{
   m_center = center;
}


void Defender::setSize(MapVec size)
{
   m_look.setSize(m_coordSys->toRenderCoords(size));
}


bool Defender::findTarget()
{
   m_target.reset();

   for (auto& attacker : *m_attackers)
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

   const MapCoord dist = glm::length(m_center - *pos);
   return sutil::lessEqual(dist, m_attribs.range);
}


void Defender::calcRotation()
{
   const auto targetDir = targetDirection();
   if (!targetDir)
      return;

   const Angle_t rot{-glm::orientedAngle(glm::normalize(*targetDir), Up)};
   m_look.setRotation(rot);
}


std::optional<MapVec> Defender::targetDirection() const
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
      (*m_target)->hit(m_attribs.damage);
}
