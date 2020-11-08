//
// Oct-2020, Michael Lindner
// MIT license
//
#include "game.h"

int main()
{
   Game towers;
   towers.setup();
   towers.run();
   towers.cleanup();
   return EXIT_SUCCESS;
}
