//
// Oct-2020, Michael Lindner
// MIT license
//
#include "game3.h"
#include "towers_game.h"

int main()
{
   Towers towers;
   //Game3 towers;
   towers.setup();
   towers.run();
   towers.cleanup();
   return EXIT_SUCCESS;
}
