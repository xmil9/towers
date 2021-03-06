//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "attacker_events.h"
#include "attacker_look.h"
#include "map_coord_sys.h"
#include "path.h"
#include "towers_types.h"
#include "essentutils/observed.h"
#include "glm/glm.hpp"
#include "glm/gtx/vector_angle.hpp"
#include <optional>


///////////////////

template <typename Derived> class AttackerBase : public esl::Observed<Derived>
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
   AttackerBase(EntityId id, AttackerLook look, sp::MapDim size, const Attribs& attribs,
                const OffsetPath& path, const MapCoordSys* cs);

   EntityId id() const { return m_id; }
   void render(sp::Renderer2& renderer, bool isPaused);
   void update();
   std::optional<sp::MapPos> position() const { return m_center; }
   void hit(int damage);
   bool isAlive() const { return m_currAttribs.hp > 0; }
   bool hasStarted() const { return m_currAttribs.launchDelay == 0; }
   bool hasFinished() const { return hasStarted() && !m_center; }
   bool canBeRemoved() const;
   int reward() const { return m_currAttribs.reward; }

 private:
   void move();
   bool isAtLastPosition() const;
   void setPosition(std::optional<sp::MapPos> center);
   void setSize(sp::MapVec size);
   void calcRotation();
   // Returns direction of movement.
   sp::MapVec direction() const;
   sp::MapVec normedDirection() const { return glm::normalize(direction()); }

   Derived& derived() { return static_cast<Derived&>(*this); }
   const Derived& derived() const { return static_cast<const Derived&>(*this); }
   const Attribs& derivedAttribs() const { return derived().m_attribs; }

 protected:
   EntityId m_id = 0;
   AttackerLook m_look;
   Attribs m_initialAttribs;
   Attribs m_currAttribs;
   std::optional<sp::MapPos> m_center;
   std::optional<Path::Index> m_currTurn;
   OffsetPath m_path;
   const MapCoordSys* m_coordSys = nullptr;
   bool m_isHit = false;
};


///////////////////

// AttackerBase implementation.

template <typename Derived>
AttackerBase<Derived>::AttackerBase(EntityId id, AttackerLook look, sp::MapDim size,
                                    const Attribs& attribs, const OffsetPath& path,
                                    const MapCoordSys* cs)
: m_id{id}, m_look{std::move(look)}, m_initialAttribs{attribs}, m_currAttribs{attribs},
  m_center{path.start()}, m_currTurn{0}, m_path{path}, m_coordSys{cs}
{
   assert(m_coordSys);

   setSize(m_coordSys->scaleInto(m_coordSys->toMap(m_look.size()), size));
   setPosition(path.start());
}


template <typename Derived>
void AttackerBase<Derived>::render(sp::Renderer2& renderer, bool isPaused)
{
   if (!hasStarted())
      return;

   const sp::PixPos center = m_coordSys->toPix(*m_center);

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
      m_look.renderExploded(renderer, center, isPaused);
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
      esl::Observed<Derived>::notify(derived(), AttackerDestroyedEvent,
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

   const sp::MapVec dir = direction();
   sp::MapVec offset = m_currAttribs.speed * glm::normalize(dir);
   // Limit movement to next turn.
   if (esl::greaterEqual(glm::length(offset), glm::length(dir)))
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
   return sp::isEqual(*m_center, m_path.finish());
}


template <typename Derived>
void AttackerBase<Derived>::setPosition(std::optional<sp::MapPos> center)
{
   m_center = center;
}


template <typename Derived> void AttackerBase<Derived>::setSize(sp::MapVec size)
{
   m_look.setSize(m_coordSys->toPix(size));
}


template <typename Derived> void AttackerBase<Derived>::calcRotation()
{
   const sp::Angle rot{glm::angle(normedDirection(), sp::Up)};
   m_look.setRotation(rot);
}


template <typename Derived> sp::MapVec AttackerBase<Derived>::direction() const
{
   if (!m_center || !m_currTurn)
      return sp::Up;

   sp::MapPos pos = *m_center;
   sp::MapPos nextPos = pos;
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
