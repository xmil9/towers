//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once


class Field
{
public:
   Field(bool attackFrom, bool buildOn);

private:
   bool m_attackFrom = false;
   bool m_buildOn = false;
};


inline Field::Field(bool attackFrom, bool buildOn)
   : m_attackFrom{attackFrom}, m_buildOn{buildOn}
{
}
