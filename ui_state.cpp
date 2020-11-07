//
// Nov-2020, Michael Lindner
// MIT license
//
#include "ui_state.h"
#include <array>


void UiState::onWindowResized(int width, int height)
{
   const glm::ivec2 newSize(width, height);
   const glm::ivec2 diff = newSize - m_wndSize;
   m_wndSize = newSize;

   if (diff.x != 0 || diff.y != 0)
   {
      WindowResizedMsgData data;
      data.size = newSize;
      notify(*this, WindowResizedMsg, data);
   }
}
