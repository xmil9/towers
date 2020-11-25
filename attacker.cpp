//
// Nov-2020, Michael Lindner
// MIT license
//
#include "attacker.h"


void Attacker::advance()
{
   if (m_pos)
      m_pos = m_path.next(*m_pos);
}
