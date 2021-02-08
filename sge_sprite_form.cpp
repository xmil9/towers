//
// Dec-2020, Michael Lindner
// MIT license
//
#include "sge_sprite_form.h"


namespace sge
{
///////////////////

void SpriteForm::setSize(PixDim size)
{
   const bool isDegenerate = m_size.x == 0.f || m_size.y == 0.f;
   const glm::vec2 rotCenterRatio =
      isDegenerate ? glm::vec2(.5f, .5f) : (m_rotCenter / m_size);
   m_size = size;
   m_rotCenter = m_size * rotCenterRatio;
}

} // namespace sge
