//
// Nov-2020, Michael Lindner
// MIT license
//
#include "path.h"
#include <algorithm>
#include <iterator>
#include <stdexcept>


///////////////////

Path::Path(const std::vector<sp::IntPos>& turns)
{
   if (turns.empty())
      throw std::runtime_error("An empty path is illegal.");

   m_turns.reserve(turns.size());
   std::transform(turns.begin(), turns.end(), std::back_inserter(m_turns),
                  [](sp::IntPos pos) {
                     constexpr sp::MapPos fieldCenter{.5f, .5f};
                     return sp::MapPos(pos) + fieldCenter;
                  });
}
