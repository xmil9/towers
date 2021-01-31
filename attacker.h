//
// Jan-2021, Michael Lindner
// MIT license
//
#pragma once
#include "assault_tank.h"
#include "coords.h"
#include "mobile_cannon.h"
#include <optional>
#include <unordered_map>
#include <variant>


///////////////////

// Wrapper around specific attacker type.
// Deals with dispatching calls to specific attacker types.
class Attacker
{
 public:
   Attacker() = default;
   template <typename SpecificAttacker> explicit Attacker(SpecificAttacker&& a);

   EntityId id() const;
   void render(Renderer2& renderer, bool isPaused);
   void update();
   std::optional<MapPos> position() const;
   void hit(int damage);
   bool isAlive() const;
   bool hasStarted() const;
   bool canBeRemoved() const;
   int reward() const;
   template <typename Callback> void addObserver(Callback cb);

 private:
   std::optional<std::variant<AssaultTank, MobileCannon>> m_attacker;
};


template <typename SpecificAttacker>
Attacker::Attacker(SpecificAttacker&& a) : m_attacker{std::move(a)}
{
}

inline EntityId Attacker::id() const
{
   if (m_attacker)
      return std::visit([&](const auto& attacker) { return attacker.id(); }, *m_attacker);
   return {};
}

inline void Attacker::render(Renderer2& renderer, bool isPaused)
{
   if (m_attacker)
      std::visit([&](auto& attacker) { attacker.render(renderer, isPaused); },
                 *m_attacker);
}

inline void Attacker::update()
{
   if (m_attacker)
      std::visit([](auto& attacker) { attacker.update(); }, *m_attacker);
}

inline std::optional<MapPos> Attacker::position() const
{
   if (m_attacker)
      return std::visit([](const auto& attacker) { return attacker.position(); },
                        *m_attacker);
   return std::nullopt;
}

inline void Attacker::hit(int damage)
{
   if (m_attacker)
      std::visit([&](auto& attacker) { attacker.hit(damage); }, *m_attacker);
}

inline bool Attacker::isAlive() const
{
   if (m_attacker)
      return std::visit([](auto& attacker) { return attacker.isAlive(); }, *m_attacker);
   return false;
}

inline bool Attacker::hasStarted() const
{
   if (m_attacker)
      return std::visit([](auto& attacker) { return attacker.hasStarted(); },
                        *m_attacker);
   return false;
}

inline bool Attacker::canBeRemoved() const
{
   if (m_attacker)
      return std::visit([](auto& attacker) { return attacker.canBeRemoved(); },
                        *m_attacker);
   return false;
}

inline int Attacker::reward() const
{
   if (m_attacker)
      return std::visit([](auto& attacker) { return attacker.reward(); }, *m_attacker);
   return 0;
}

template <typename Callback> void Attacker::addObserver(Callback cb)
{
   if (m_attacker)
      std::visit([cb](auto& attacker) { attacker.addObserver(cb); }, *m_attacker);
}


///////////////////

// Wrapper around specific attacker attributes types.
// Deals with dispatching calls to specific attributes types.
class AttackerAttribs
{
 public:
   AttackerAttribs() = default;
   template <typename SpecificAttribs> explicit AttackerAttribs(SpecificAttribs&& a);

   // Common attributes.
   int hp() const;
   float speed() const;
   int launchDelay() const;
   int reward() const;

   // Access to attributes of given type.
   template <typename Attribs> Attribs get() const;

 private:
   std::optional<std::variant<AssaultTank::Attribs, MobileCannon::Attribs>> m_attribs;
};


template <typename SpecificAttribs>
AttackerAttribs::AttackerAttribs(SpecificAttribs&& a) : m_attribs{std::move(a)}
{
}

inline int AttackerAttribs::hp() const
{
   if (m_attribs)
      return std::visit([](const auto& attribs) { return attribs.hp; }, *m_attribs);
   return 0;
}

inline float AttackerAttribs::speed() const
{
   if (m_attribs)
      return std::visit([](const auto& attribs) { return attribs.speed; }, *m_attribs);
   return 0.f;
}

inline int AttackerAttribs::launchDelay() const
{
   if (m_attribs)
      return std::visit([](const auto& attribs) { return attribs.launchDelay; },
                        *m_attribs);
   return 0;
}

inline int AttackerAttribs::reward() const
{
   if (m_attribs)
      return std::visit([](const auto& attribs) { return attribs.reward; }, *m_attribs);
   return 0;
}

template <typename Attribs> Attribs AttackerAttribs::get() const
{
   if (m_attribs)
      return std::get<Attribs>(*m_attribs);
   return {};
}


///////////////////

using AttackerMap = std::unordered_map<EntityId, Attacker>;