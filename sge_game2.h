//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "sge_coords.h"
#include <string>


namespace sge
{
///////////////////

class Game2
{
 public:
   Game2(PixCoordi wndWidth, PixCoordi wndHeight, const std::string& wndTitle);

 private:
   PixCoordi m_wndWidth = 0;
   PixCoordi m_wndHeight = 0;
   std::string m_wndTitle;
};

} // namespace sge
