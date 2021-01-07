//
// Dec-2020, Michael Lindner
// MIT license
//
#pragma once
#include "attacker.h"
#include "defender.h"
#include "defender_look.h"
#include "defender_models.h"
#include <cassert>
#include <string>
#include <unordered_map>
#include <vector>

class MapCoordSys;


///////////////////

// Simplifies creating defender instances by storing some data that is always the same
// for either all defender models (e.g. map coordinate system) or for all instances of one
// defender model (e.g. sprites).
class DefenderFactory
{
 public:
   explicit DefenderFactory(const MapCoordSys* cs, std::vector<Attacker>* attackers);

   void registerModel(const std::string& name, DefenderLook look);

   Defender makeLaserTurret(MapCoord size, MapPos center,
                            const LaserTurret::Attribs& attribs) const;
   Defender makeSonicMortar(MapCoord size, MapPos center,
                            const SonicMortar::Attribs& attribs) const;

 private:
   struct Model
   {
      std::string name;
      DefenderLook look;
   };

   const Model& lookupModel(const std::string& modelName) const;

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
