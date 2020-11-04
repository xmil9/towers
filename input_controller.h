//
// Oct-2020, Michael Lindner
// MIT license
//
#pragma once


struct InputController
{
   virtual void onMouseMoved(double xpos, double ypos) = 0;
   virtual void onMouseScrolled(double xoffset, double yoffset) = 0;
   virtual void onKeyChanged(int key, int scancode, int action, int mods) = 0;
};
