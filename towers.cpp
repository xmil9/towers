//
// Oct-2020, Michael Lindner
// MIT license
//
#include "game3.h"

int main()
{
   Game3 towers;
   towers.setup();
   towers.run();
   towers.cleanup();
   return EXIT_SUCCESS;
}
