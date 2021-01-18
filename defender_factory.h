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
#include <variant>
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

   template <typename Attribs>
   void registerModel(const std::string& name, DefenderLook look, Attribs&& attribs);

   Defender makeDefender(const std::string& model, MapPos center) const;
   DefenderAttribs defaultAttributes(const std::string& model) const;

 private:
   struct Model
   {
      std::string name;
      DefenderLook look;
      DefenderAttribs attribs;
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

template <typename Attribs>
void DefenderFactory::registerModel(const std::string& name, DefenderLook look,
                                    Attribs&& attribs)
{
   m_models.insert(
      std::pair{name, Model{name, std::move(look), DefenderAttribs{std::move(attribs)}}});
}

inline DefenderAttribs DefenderFactory::defaultAttributes(const std::string& model) const
{
   const Model& data = lookupModel(model);
   return data.attribs;
}
