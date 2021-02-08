//
// Dec-2020, Michael Lindner
// MIT license
//
#include "sprite_form.h"


void SpriteForm::setSize(sge::PixDim size)
{
   const bool isDegenerate = m_size.x == 0.f || m_size.y == 0.f;
   const glm::vec2 rotCenterRatio =
      isDegenerate ? glm::vec2(.5f, .5f) : (m_rotCenter / m_size);
   m_size = size;
   m_rotCenter = m_size * rotCenterRatio;
}
