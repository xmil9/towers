//
// Oct-2020, Michael Lindner
// MIT license
//
#include "game2.h"
#include "game3.h"

int main()
{
   Game2 towers;
   //Game3 towers;
   towers.setup();
   towers.run();
   towers.cleanup();
   return EXIT_SUCCESS;
}
