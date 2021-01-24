//
// Dec-2020, Michael Lindner
// MIT license
//
#include "attacker_factory.h"


///////////////////

const AttackerFactory::Model&
AttackerFactory::lookupModel(const std::string& modelName) const
{
   const auto pos = m_models.find(modelName);
   assert(pos != m_models.end());
   if (pos == m_models.end())
      throw std::runtime_error{"Configuration failure. Model not registered."};
   return pos->second;
}


Attacker AttackerFactory::makeAttacker(const std::string& model, const OffsetPath& path,
                                       int launchDelay) const
{
   const Model& data = lookupModel(model);

   if (model == AatModel)
   {
      AssaultTank::Attribs attribs = data.attribs.get<AssaultTank::Attribs>();
      attribs.launchDelay = launchDelay;

      return Attacker{AssaultTank{data.look, AatSize, attribs, path, m_coordSys}};
   }
   else if (model == MhcModel)
   {
      MobileCannon::Attribs attribs = data.attribs.get<MobileCannon::Attribs>();
      attribs.launchDelay = launchDelay;

      return Attacker{MobileCannon{data.look, MhcSize, attribs, path, m_coordSys}};
   }

   assert(false && "Unknown defender model.");
   return {};
}
