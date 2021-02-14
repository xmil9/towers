//
// Dec-2020, Michael Lindner
// MIT license
//
#pragma once
#include "attacker.h"
#include "defender_look.h"
#include "map_coord_sys.h"
#include "path.h"
#include "target_scan.h"
#include "sge_renderer2.h"
#include "essentutils/fputil.h"
#include "glm/glm.hpp"
#include "glm/gtx/vector_angle.hpp"
#include <cstddef>
#include <limits>
#include <optional>
#include <unordered_map>


///////////////////

template <typename Derived> class DefenderBase
{
 public:
   struct Attribs
   {
      sge::MapCoord range = 0.f;
      int damage = 0;
      int cost = 0;
   };

 public:
   DefenderBase(EntityId id, DefenderLook look, sge::MapDim size, sge::MapPos center,
                const MapCoordSys* cs, AttackerMap* attackers);

   EntityId id() const { return m_id; }
   int cost() const { return baseAttribs().cost; }
   sge::MapCoord range() const { return baseAttribs().range; }
   sge::MapPos center() const { return m_center; }
   void render(sge::Renderer2& renderer, bool isPaused);
   void update();
   void removeAsTarget(EntityId attackerId);
   bool isInRange(const Attacker& attacker) const;
   bool isInRange(sge::MapCoord dist) const;

 protected:
   Attacker& target();
   const Attacker& target() const;

 private:
   void setPosition(sge::MapPos center);
   void setSize(sge::MapVec size);
   bool findTarget();
   bool canHit(const Attacker& attacker) const;
   void calcRotation();
   std::optional<sge::MapVec> targetDirection() const;

   Derived& derived() { return static_cast<Derived&>(*this); }
   const Derived& derived() const { return static_cast<const Derived&>(*this); }
   const Attribs& baseAttribs() const { return derived().m_attribs; }

 protected:
   EntityId m_id = 0;
   DefenderLook m_look;
   sge::MapPos m_center;
   const MapCoordSys* m_coordSys;
   AttackerMap* m_attackers = nullptr;
   ClosestTargetScan<Derived> m_targetScan;
   std::optional<EntityId> m_target;
};


template <typename Derived>
DefenderBase<Derived>::DefenderBase(EntityId id, DefenderLook look, sge::MapDim size,
                                    sge::MapPos center, const MapCoordSys* cs,
                                    AttackerMap* attackers)
: m_id{id}, m_look{std::move(look)}, m_center{center}, m_coordSys{cs},
  m_attackers{attackers}, m_targetScan{attackers}
{
   assert(m_coordSys);
   assert(m_attackers);

   setSize(m_coordSys->scaleInto(m_coordSys->toMap(m_look.size()), size));
   setPosition(center);
}


template <typename Derived>
void DefenderBase<Derived>::render(sge::Renderer2& renderer, bool isPaused)
{
   const sge::PixPos center = m_coordSys->toPix(m_center);

   if (m_target && target().isAlive())
   {
      calcRotation();
      m_look.renderFiring(renderer, center, isPaused);
   }
   else
   {
      m_look.render(renderer, center);
   }
}


template <typename Derived> Attacker& DefenderBase<Derived>::target()
{
   assert(m_target.has_value());
   return (*m_attackers)[*m_target];
}


template <typename Derived> const Attacker& DefenderBase<Derived>::target() const
{
   assert(m_target.has_value());
   return (*m_attackers)[*m_target];
}


template <typename Derived> void DefenderBase<Derived>::update()
{
   if (findTarget())
      derived().shoot();
}


template <typename Derived>
void DefenderBase<Derived>::removeAsTarget(EntityId attackerId)
{
   if (m_target && target().id() == attackerId)
      m_target = std::nullopt;
}


template <typename Derived> void DefenderBase<Derived>::setPosition(sge::MapPos center)
{
   m_center = center;
}


template <typename Derived> void DefenderBase<Derived>::setSize(sge::MapVec size)
{
   m_look.setSize(m_coordSys->toPix(size));
}


template <typename Derived> bool DefenderBase<Derived>::findTarget()
{
   // Try to keep existing target.
   if (m_target && canHit(target()))
      return true;

   // Find new target.
   m_target = m_targetScan.scan(derived());
   return m_target.has_value();
}


template <typename Derived>
bool DefenderBase<Derived>::canHit(const Attacker& attacker) const
{
   return attacker.isAlive() && isInRange(attacker);
}


template <typename Derived>
bool DefenderBase<Derived>::isInRange(const Attacker& attacker) const
{
   const auto pos = attacker.position();
   if (pos)
      return isInRange(glm::length(m_center - *pos));
   return false;
}


template <typename Derived>
bool DefenderBase<Derived>::isInRange(sge::MapCoord dist) const
{
   return esl::lessEqual(dist, baseAttribs().range);
}


template <typename Derived> void DefenderBase<Derived>::calcRotation()
{
   const auto targetDir = targetDirection();
   if (!targetDir)
      return;

   const sge::Angle rot{-glm::orientedAngle(glm::normalize(*targetDir), sge::Up)};
   m_look.setRotation(rot);
}


template <typename Derived>
std::optional<sge::MapVec> DefenderBase<Derived>::targetDirection() const
{
   if (!m_target)
      return std::nullopt;

   const auto targetPos = target().position();
   if (!targetPos)
      return std::nullopt;

   return *targetPos - m_center;
}
