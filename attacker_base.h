//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "attacker_events.h"
#include "attacker_look.h"
#include "map_coord_sys.h"
#include "observed.h"
#include "path.h"
#include "glm/glm.hpp"
#include "glm/gtx/vector_angle.hpp"
#include <optional>

class Renderer2;


///////////////////

template <typename Derived> class AttackerBase : public Observed<Derived>
{
 public:
   struct Attribs
   {
      int hp = 0;
      float speed = .1f;
      int launchDelay = 0;
      int reward = 0;
   };

 public:
   AttackerBase(AttackerLook look, MapCoord size, const Attribs& attribs,
                const OffsetPath& path, const MapCoordSys* cs);

   void render(Renderer2& renderer);
   void update();
   std::optional<MapPos> position() const { return m_center; }
   void hit(int damage);
   bool isAlive() const { return m_currAttribs.hp > 0; }
   bool hasStarted() const { return m_currAttribs.launchDelay == 0; }
   bool canBeRemoved() const;
   int reward() const { return m_currAttribs.reward; }

 private:
   void move();
   bool isAtLastPosition() const;
   void setPosition(std::optional<MapPos> center);
   void setSize(MapVec size);
   void calcRotation();
   // Returns direction of movement.
   MapVec direction() const;
   MapVec normedDirection() const { return glm::normalize(direction()); }

   Derived& derived() { return static_cast<Derived&>(*this); }
   const Derived& derived() const { return static_cast<const Derived&>(*this); }
   const Attribs& derivedAttribs() const { return derived().m_attribs; }

 protected:
   AttackerLook m_look;
   Attribs m_initialAttribs;
   Attribs m_currAttribs;
   std::optional<MapPos> m_center;
   std::optional<Path::Index> m_currTurn;
   OffsetPath m_path;
   const MapCoordSys* m_coordSys = nullptr;
   bool m_isHit = false;
};


///////////////////

// AttackerBase implementation.

template <typename Derived>
AttackerBase<Derived>::AttackerBase(AttackerLook look, MapCoord size,
                                    const Attribs& attribs, const OffsetPath& path,
                                    const MapCoordSys* cs)
: m_look{std::move(look)}, m_initialAttribs{attribs}, m_currAttribs{attribs},
  m_center{path.start()}, m_currTurn{0}, m_path{path}, m_coordSys{cs}
{
   assert(m_coordSys);

   setSize(m_coordSys->scaleToSize(size, m_look.size()));
   setPosition(path.start());
}


template <typename Derived> void AttackerBase<Derived>::render(Renderer2& renderer)
{
   if (!hasStarted())
      return;

   const PixPos center = m_coordSys->toRenderCoords(*m_center);

   if (isAlive())
   {
      calcRotation();
      m_look.render(renderer, center, true,
                    static_cast<float>(m_currAttribs.hp) /
                       static_cast<float>(m_initialAttribs.hp),
                    m_isHit);
   }
   else
   {
      m_look.renderExploded(renderer, center);
   }
}


template <typename Derived> void AttackerBase<Derived>::update()
{
   m_isHit = false;
   if (m_currAttribs.launchDelay == 0)
      move();
   else
      --m_currAttribs.launchDelay;
}


template <typename Derived> void AttackerBase<Derived>::hit(int damage)
{
   if (!isAlive())
      return;

   m_currAttribs.hp = std::max(0, m_currAttribs.hp - damage);
   m_isHit = true;

   if (!isAlive())
      Observed<Derived>::notify(derived(), AttackerDestroyedEvent,
                                AttackerDestroyedData{});
}


template <typename Derived> bool AttackerBase<Derived>::canBeRemoved() const
{
   const bool isAtGoal = !m_center;
   const bool hasExploded = m_look.hasExplosionFinished();
   return isAtGoal || hasExploded;
}


template <typename Derived> void AttackerBase<Derived>::move()
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

   const MapVec dir = direction();
   MapVec offset = m_currAttribs.speed * glm::normalize(dir);
   // Limit movement to next turn.
   if (sutil::greaterEqual(glm::length(offset), glm::length(dir)))
   {
      offset = dir;
      ++*m_currTurn;
   }

   setPosition(*m_center + offset);
}


template <typename Derived> bool AttackerBase<Derived>::isAtLastPosition() const
{
   if (!m_center)
      return true;
   return isEqual(*m_center, m_path.finish());
}


template <typename Derived>
void AttackerBase<Derived>::setPosition(std::optional<MapPos> center)
{
   m_center = center;
}


template <typename Derived> void AttackerBase<Derived>::setSize(MapVec size)
{
   m_look.setSize(m_coordSys->toRenderCoords(size));
}


template <typename Derived> void AttackerBase<Derived>::calcRotation()
{
   const Angle_t rot{glm::angle(normedDirection(), Up)};
   m_look.setRotation(rot);
}


template <typename Derived> MapVec AttackerBase<Derived>::direction() const
{
   if (!m_center || !m_currTurn)
      return Up;

   MapPos pos = *m_center;
   MapPos nextPos = pos;
   if (*m_currTurn < m_path.size() - 1)
   {
      nextPos = m_path[*m_currTurn + 1];
   }
   else
   {
      pos = m_path[*m_currTurn - 1];
      nextPos = m_path[*m_currTurn];
   }

   return nextPos - pos;
}
