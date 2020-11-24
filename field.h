//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include <cstdint>


///////////////////

class Field
{
public:
   using Property = int64_t;
   using Properties = int64_t;
   static constexpr Property AttackFrom = 0x00000001;
   static constexpr Property BuildOn = 0x00000002;

public:
   Field() = default;
   explicit Field(Properties props);

private:
   Properties m_props = 0;
};


inline Field::Field(Properties props)
   : m_props{props}
{
}


///////////////////

inline Field makePathField()
{
   return Field{Field::AttackFrom};
}

inline Field makeSiteField()
{
   return Field{Field::BuildOn};
}

inline Field makeOffField()
{
   return Field{};
}
