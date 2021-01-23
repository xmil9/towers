//
// Dec-2020, Michael Lindner
// MIT license
//
#pragma once
#include "attacker.h"
#include "defender_look.h"
#include "map_coord_sys.h"
#include "path.h"
#include "renderer2.h"
#include "essentutils/fputil.h"
#include "glm/glm.hpp"
#include "glm/gtx/vector_angle.hpp"
#include <cstddef>
#include <optional>
#include <vector>


///////////////////

template <typename Derived> class DefenderBase
{
 public:
   struct Attribs
   {
      MapCoord range = 0.f;
      int damage = 0;
      int cost = 0;
   };

 public:
   DefenderBase(DefenderLook look, MapCoord size, MapPos center, const MapCoordSys* cs,
                std::vector<Attacker>* attackers);

   int cost() const { return derivedAttribs().cost; }
   void render(Renderer2& renderer);
   void update();

 private:
   void setPosition(MapPos center);
   void setSize(MapVec size);
   bool findTarget();
   bool isInRange(const Attacker& attacker) const;
   void calcRotation();
   std::optional<MapVec> targetDirection() const;

   Derived& derived() { return static_cast<Derived&>(*this); }
   const Derived& derived() const { return static_cast<const Derived&>(*this); }
   const Attribs& derivedAttribs() const { return derived().m_attribs; }

 protected:
   DefenderLook m_look;
   MapPos m_center;
   const MapCoordSys* m_coordSys;
   std::vector<Attacker>* m_attackers = nullptr;
   std::optional<Attacker*> m_target;
};


template <typename Derived>
DefenderBase<Derived>::DefenderBase(DefenderLook look, MapCoord size, MapPos center,
                                    const MapCoordSys* cs,
                                    std::vector<Attacker>* attackers)
: m_look{std::move(look)}, m_center{center}, m_coordSys{cs}, m_attackers{attackers}
{
   assert(m_coordSys);
   assert(m_attackers);

   setSize(m_coordSys->scaleToSize(size, m_look.size()));
   setPosition(center);
}


template <typename Derived>
void DefenderBase<Derived>::render(Renderer2& renderer)
{
   const PixPos center = m_coordSys->toRenderCoords(m_center);

   if (m_target)
   {
      calcRotation();
      m_look.renderFiring(renderer, center);
   }
   else
   {
      m_look.render(renderer, center);
   }
}


template <typename Derived> void DefenderBase<Derived>::update()
{
   if (findTarget())
      derived().shoot();
}


template <typename Derived> void DefenderBase<Derived>::setPosition(MapPos center)
{
   m_center = center;
}


template <typename Derived> void DefenderBase<Derived>::setSize(MapVec size)
{
   m_look.setSize(m_coordSys->toRenderCoords(size));
}


template <typename Derived> bool DefenderBase<Derived>::findTarget()
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


template <typename Derived>
bool DefenderBase<Derived>::isInRange(const Attacker& attacker) const
{
   const auto pos = attacker.position();
   if (!pos)
      return false;

   const MapCoord dist = glm::length(m_center - *pos);
   return sutil::lessEqual(dist, derivedAttribs().range);
}


template <typename Derived> void DefenderBase<Derived>::calcRotation()
{
   const auto targetDir = targetDirection();
   if (!targetDir)
      return;

   const Angle_t rot{-glm::orientedAngle(glm::normalize(*targetDir), Up)};
   m_look.setRotation(rot);
}


template <typename Derived>
std::optional<MapVec> DefenderBase<Derived>::targetDirection() const
{
   if (!m_target)
      return std::nullopt;

   const auto targetPos = (*m_target)->position();
   if (!targetPos)
      return std::nullopt;

   return *targetPos - m_center;
}
