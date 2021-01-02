//
// Dec-2020, Michael Lindner
// MIT license
//
#include "defender_factory.h"


///////////////////

std::optional<Defender>
DefenderFactory::makeDefender(const std::string& model, MapCoord size, MapPos center,
                              const Defender::Attribs& attribs) const
{
   const auto pos = m_models.find(model);
   if (pos != m_models.end())
      return Defender{pos->second.look, size, center, attribs, m_coordSys, m_attackers};
   return std::nullopt;
}
