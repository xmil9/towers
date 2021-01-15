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
// Deals with dispatching calls to specific defender type.
class Defender
{
 public:
   Defender() = default;
   template <typename SpecificDefender> explicit Defender(SpecificDefender&& d);

   void render(Renderer2& renderer);
   void update();

 private:
   std::optional<std::variant<LaserTurret, SonicMortar>> m_defender;
};


template <typename SpecificDefender>
Defender::Defender(SpecificDefender&& d) : m_defender{std::move(d)}
{
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
