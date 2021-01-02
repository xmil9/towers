//
// Dec-2020, Michael Lindner
// MIT license
//
#pragma once
#include "attacker.h"
#include "attacker_look.h"
#include <optional>
#include <string>
#include <unordered_map>

class MapCoordSys;


///////////////////

class AttackerFactory
{
 public:
   explicit AttackerFactory(const MapCoordSys* cs);

   void registerModel(const std::string& name, AttackerLook look);
   std::optional<Attacker> makeAttacker(const std::string& model, MapCoord size,
                                        const Attacker::Attribs& attribs,
                                        const OffsetPath& path) const;

 private:
   struct Model
   {
      std::string name;
      AttackerLook look;
   };

 private:
   const MapCoordSys* m_coordSys = nullptr;
   std::unordered_map<std::string, Model> m_models;
};


inline AttackerFactory::AttackerFactory(const MapCoordSys* cs) : m_coordSys{cs}
{
}

inline void AttackerFactory::registerModel(const std::string& name, AttackerLook look)
{
   m_models.insert(std::pair{name, Model{name, std::move(look)}});
}
