//
// Dec-2020, Michael Lindner
// MIT license
//
#include "attacker_factory.h"


///////////////////

std::optional<Attacker> AttackerFactory::makeAttacker(const std::string& model,
                                                      MapCoord size,
                                                      const Attacker::Attribs& attribs,
                                                      const OffsetPath& path) const
{
   const auto pos = m_models.find(model);
   if (pos != m_models.end())
      return Attacker{pos->second.look, size, attribs, path, m_coordSys};
   return std::nullopt;
}
