//
// Dec-2020, Michael Lindner
// MIT license
//
#pragma once
#include "attacker.h"
#include "attacker_look.h"
#include <string>
#include <unordered_map>

class MapCoordSys;
class OffsetPath;


///////////////////

class AttackerFactory
{
 public:
   explicit AttackerFactory(const MapCoordSys* cs);

   template <typename Attribs>
   void registerModel(const std::string& name, AttackerLook look, Attribs&& attribs);

   Attacker makeAttacker(const std::string& model, const OffsetPath& path,
                         int launchDelay = 0);
   AttackerAttribs defaultAttributes(const std::string& model) const;

 private:
   struct Model
   {
      std::string name;
      AttackerLook look;
      AttackerAttribs attribs;
   };

   const Model& lookupModel(const std::string& modelName) const;

 private:
   EntityId m_nextId = 1;
   const MapCoordSys* m_coordSys = nullptr;
   std::unordered_map<std::string, Model> m_models;
};


inline AttackerFactory::AttackerFactory(const MapCoordSys* cs) : m_coordSys{cs}
{
}

template <typename Attribs>
void AttackerFactory::registerModel(const std::string& name, AttackerLook look,
                                    Attribs&& attribs)
{
   m_models.insert(
      std::pair{name, Model{name, std::move(look), AttackerAttribs{std::move(attribs)}}});
}

inline AttackerAttribs AttackerFactory::defaultAttributes(const std::string& model) const
{
   const Model& data = lookupModel(model);
   return data.attribs;
}
