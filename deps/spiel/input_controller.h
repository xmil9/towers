//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "opengl_util/gfl_types.h"


namespace sp
{

struct InputController
{
   virtual void onMouseMoved(double xpos, double ypos) = 0;
   virtual void onMouseScrolled(double xoffset, double yoffset) = 0;
   virtual void onMouseButtonChanged(gfl::MouseButton button, int action, int mods,
                                     double xpos, double ypos) = 0;
   virtual void onKeyChanged(gfl::Key key, int scancode, int action, int mods) = 0;
};

} // namespace sp
