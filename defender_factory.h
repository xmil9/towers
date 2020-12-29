//
// Dec-2020, Michael Lindner
// MIT license
//
#pragma once
#include "attacker.h"
#include "defender.h"
#include "defender_look.h"
#include <cassert>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

class MapCoordSys;


///////////////////

class DefenderFactory
{
 public:
   explicit DefenderFactory(const MapCoordSys* cs, std::vector<Attacker>* attackers);

   void registerModel(const std::string& name, DefenderLook look);
   std::optional<Defender> makeDefender(const std::string& model, NormCoord size,
                                        NormPos center,
                                        const Defender::Attribs& attribs) const;

 private:
   struct Model
   {
      std::string name;
      DefenderLook look;
   };

 private:
   const MapCoordSys* m_coordSys = nullptr;
   // Collection of active attackers.
   std::vector<Attacker>* m_attackers = nullptr;
   std::unordered_map<std::string, Model> m_models;
};


inline DefenderFactory::DefenderFactory(const MapCoordSys* cs,
                                        std::vector<Attacker>* attackers)
: m_coordSys{cs}, m_attackers{attackers}
{
   assert(m_coordSys);
   assert(attackers);
}

inline void DefenderFactory::registerModel(const std::string& name, DefenderLook look)
{
   m_models.insert(std::pair{name, Model{name, std::move(look)}});
}
