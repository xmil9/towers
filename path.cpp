//
// Nov-2020, Michael Lindner
// MIT license
//
#include "path.h"
#include <algorithm>
#include <iterator>
#include <stdexcept>


///////////////////

Path::Path(const std::vector<sge::IntPos>& turns)
{
   if (turns.empty())
      throw std::runtime_error("An empty path is illegal.");

   m_turns.reserve(turns.size());
   std::transform(turns.begin(), turns.end(), std::back_inserter(m_turns),
                  [](sge::IntPos pos) {
                     constexpr sge::MapPos fieldCenter{.5f, .5f};
                     return sge::MapPos(pos) + fieldCenter;
                  });
}
