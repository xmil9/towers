//
// Jan-2021, Michael Lindner
// MIT license
//
#pragma once
#include "coords.h"
#include <string>


///////////////////

struct Commands
{
   virtual void startPlaceSession(std::string_view model, std::string_view indicatorTex,
                                  PixDim indicatorDim) = 0;
   virtual void endPlaceSession() = 0;

 protected:
   // No destruction through this interface.
   ~Commands() = default;
};
