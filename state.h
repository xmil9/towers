//
// Jan-2021, Michael Lindner
// MIT license
//
#pragma once


///////////////////

struct State
{
   virtual int credits() const = 0;

 protected:
   // No destruction through this interface.
   ~State() = default;
};
