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
      throw std::runtime_error{
         "Configuration failure. Laser turrent model not registered."};
   return pos->second;
}


Defender DefenderFactory::makeLaserTurret(MapCoord size, MapPos center,
                                          const LaserTurret::Attribs& attribs) const
{
   const Model& data = lookupModel(LtModel);
   return Defender{
      LaserTurret{data.look, size, center, attribs, m_coordSys, m_attackers}};
}


Defender DefenderFactory::makeSonicMortar(MapCoord size, MapPos center,
                                          const SonicMortar::Attribs& attribs) const
{
   const Model& data = lookupModel(SmModel);
   return Defender{
      SonicMortar{data.look, size, center, attribs, m_coordSys, m_attackers}};
}
