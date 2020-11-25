//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "terrain_types.h"
#include <optional>
#include <vector>


class Path
{
public:
   Path() = default;
   explicit Path(const std::vector<FieldPos>& turns);

   std::optional<FieldPos> next(const FieldPos& from) const;

private:
   std::vector<FieldPos> m_fields;
};
