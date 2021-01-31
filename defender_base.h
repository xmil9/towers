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
#include <limits>
#include <optional>
#include <unordered_map>


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
   DefenderBase(EntityId id, DefenderLook look, MapCoord size, MapPos center,
                const MapCoordSys* cs, AttackerMap* attackers);

   EntityId id() const { return m_id; }
   int cost() const { return baseAttribs().cost; }
   MapCoord range() const { return baseAttribs().range; }
   MapPos center() const { return m_center; }
   void render(Renderer2& renderer);
   void update();
   void removeAsTarget(EntityId attackerId);

 protected:
   Attacker& target();
   const Attacker& target() const;

 private:
   void setPosition(MapPos center);
   void setSize(MapVec size);
   bool findTarget();
   std::optional<MapCoord> distTo(const Attacker& attacker) const;
   bool canHit(const Attacker& attacker) const;
   bool isInRange(const Attacker& attacker) const;
   bool isInRange(MapCoord dist) const;
   void calcRotation();
   std::optional<MapVec> targetDirection() const;

   Derived& derived() { return static_cast<Derived&>(*this); }
   const Derived& derived() const { return static_cast<const Derived&>(*this); }
   const Attribs& baseAttribs() const { return derived().m_attribs; }

 protected:
   EntityId m_id = 0;
   DefenderLook m_look;
   MapPos m_center;
   const MapCoordSys* m_coordSys;
   AttackerMap* m_attackers = nullptr;
   std::optional<EntityId> m_target;
};


template <typename Derived>
DefenderBase<Derived>::DefenderBase(EntityId id, DefenderLook look, MapCoord size,
                                    MapPos center, const MapCoordSys* cs,
                                    AttackerMap* attackers)
: m_id{id}, m_look{std::move(look)}, m_center{center}, m_coordSys{cs}, m_attackers{
                                                                          attackers}
{
   assert(m_coordSys);
   assert(m_attackers);

   setSize(m_coordSys->scaleToSize(size, m_look.size()));
   setPosition(center);
}


template <typename Derived> void DefenderBase<Derived>::render(Renderer2& renderer)
{
   const PixPos center = m_coordSys->toRenderCoords(m_center);

   if (m_target && target().isAlive())
   {
      calcRotation();
      m_look.renderFiring(renderer, center);
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
   // Try to keep existing target.
   if (m_target && canHit(target()))
      return true;

   // Find new target.
   const auto iter =
      std::find_if(m_attackers->begin(), m_attackers->end(), [this](const auto& entry) {
         return entry.second.isAlive() && canHit(entry.second);
      });
   m_target =
      iter != m_attackers->end() ? std::make_optional(iter->second.id()) : std::nullopt;

   return m_target.has_value();
   // m_target = std::nullopt;
   // MapCoord minDist = std::numeric_limits<MapCoord>::max();

   // for (auto& attacker : *m_attackers)
   //{
   //   if (attacker.isAlive())
   //   {
   //      if (const auto dist = distTo(attacker);
   //          dist && isInRange(*dist) && sutil::lessEqual(*dist, minDist))
   //      {
   //         m_target = &attacker;
   //         minDist = *dist;
   //      }
   //   }
   //}

   // return m_target.has_value();
}


template <typename Derived>
std::optional<MapCoord> DefenderBase<Derived>::distTo(const Attacker& attacker) const
{
   const auto pos = attacker.position();
   if (pos)
      return glm::length(m_center - *pos);
   return std::nullopt;
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


template <typename Derived> bool DefenderBase<Derived>::isInRange(MapCoord dist) const
{
   return sutil::lessEqual(dist, baseAttribs().range);
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

   const auto targetPos = target().position();
   if (!targetPos)
      return std::nullopt;

   return *targetPos - m_center;
}
