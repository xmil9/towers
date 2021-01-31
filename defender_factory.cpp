//
// Dec-2020, Michael Lindner
// MIT license
//
#include "defender_factory.h"


///////////////////

const DefenderFactory::Model&
DefenderFactory::lookupModel(const std::string& modelName) const
{
   const auto pos = m_models.find(modelName);
   assert(pos != m_models.end());
   if (pos == m_models.end())
      throw std::runtime_error{"Configuration failure. Model not registered."};
   return pos->second;
}


Defender DefenderFactory::makeDefender(const std::string& model, MapPos center)
{
   EntityId id = m_nextId++;
   const Model& data = lookupModel(model);

   if (model == LtModel)
   {
      return Defender{LaserTurret{id, data.look, LtSize, center,
                                  data.attribs.get<LaserTurret::Attribs>(), m_coordSys,
                                  m_attackers}};
   }
   else if (model == SmModel)
   {
      return Defender{SonicMortar{id, data.look, SmSize, center,
                                  data.attribs.get<SonicMortar::Attribs>(), m_coordSys,
                                  m_attackers}};
   }

   assert(false && "Unknown defender model.");
   return {};
}
