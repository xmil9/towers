//
// Jan-2021, Michael Lindner
// MIT license
//
#pragma once
#include <string>


///////////////////

struct State
{
   virtual int credits() const = 0;
   virtual bool canAffordDefender(const std::string& model) const = 0;
   virtual bool canStartAttack() const = 0;

 protected:
   // No destruction through this interface.
   ~State() = default;
};
