//
// Jan-2021, Michael Lindner
// MIT license
//
#pragma once
#include "laser_turret.h"
#include "renderer2.h"
#include "sonic_mortar.h"
#include <optional>
#include <variant>


///////////////////

// Wrapper around specific defender type.
// Deals with dispatching calls to specific defender types.
class Defender
{
 public:
   Defender() = default;
   template <typename SpecificDefender> explicit Defender(SpecificDefender&& d);

   int cost() const;
   void render(Renderer2& renderer);
   void update();

 private:
   std::optional<std::variant<LaserTurret, SonicMortar>> m_defender;
};


template <typename SpecificDefender>
Defender::Defender(SpecificDefender&& d) : m_defender{std::move(d)}
{
}

inline int Defender::cost() const
{
   if (m_defender)
      return std::visit([](const auto& defender) { return defender.cost(); },
                        *m_defender);
   return 0;
}

inline void Defender::render(Renderer2& renderer)
{
   if (m_defender)
      std::visit([&renderer](auto& defender) { defender.render(renderer); }, *m_defender);
}

inline void Defender::update()
{
   if (m_defender)
      std::visit([](auto& defender) { defender.update(); }, *m_defender);
}


///////////////////

// Wrapper around specific defender attributes type.
// Deals with dispatching calls to specific attributes types.
class DefenderAttribs
{
 public:
   DefenderAttribs() = default;
   template <typename SpecificAttribs> explicit DefenderAttribs(SpecificAttribs&& a);

   // Common attributes.
   MapCoord range() const;
   int damage() const;
   int cost() const;

   // Access to attributes of given type.
   template <typename Attribs> Attribs get() const;

 private:
   std::optional<std::variant<LaserTurret::Attribs, SonicMortar::Attribs>> m_attribs;
};


template <typename SpecificAttribs>
DefenderAttribs::DefenderAttribs(SpecificAttribs&& a) : m_attribs{std::move(a)}
{
}

inline MapCoord DefenderAttribs::range() const
{
   if (m_attribs)
      return std::visit([](const auto& attribs) { return attribs.range; }, *m_attribs);
   return 0.f;
}

inline int DefenderAttribs::damage() const
{
   if (m_attribs)
      return std::visit([](const auto& attribs) { return attribs.damage; }, *m_attribs);
   return 0;
}

inline int DefenderAttribs::cost() const
{
   if (m_attribs)
      return std::visit([](const auto& attribs) { return attribs.cost; }, *m_attribs);
   return 0;
}

template <typename Attribs> Attribs DefenderAttribs::get() const
{
   if (m_attribs)
      return std::get<Attribs>(*m_attribs);
   return {};
}
