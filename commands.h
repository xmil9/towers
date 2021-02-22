//
// Jan-2021, Michael Lindner
// MIT license
//
#pragma once
#include "spiel/coords.h"
#include <string>


///////////////////

struct Commands
{
   virtual void startPlaceSession(std::string_view model, std::string_view indicatorTex,
                                  sp::PixDim indicatorDim) = 0;
   virtual void endPlaceSession() = 0;
   virtual void startAttack() = 0;
   virtual void pauseAttack() = 0;
   virtual void playAgain() = 0;
   virtual void advanceLevel() = 0;
   virtual void repeatLevel() = 0;
   virtual void quit() = 0;

 protected:
   // No destruction through this interface.
   ~Commands() = default;
};
