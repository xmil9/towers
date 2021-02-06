//
// Oct-2020, Michael Lindner
// MIT license
//
#pragma once
#include "coords.h"
#include "input_controller.h"
#include "essentutils/observed.h"
#include "glm/vec2.hpp"
#include "opengl_util/gfl_window.h"


class MainWindow : public gfl::Window, public sutil::Observed<MainWindow>
{
 public:
   void setInputController(InputController* controller);
   PixPos mousePosition() const;

 protected:
   void onWindowResized(int width, int height) override;
   void onWindowMouseMoved(double xpos, double ypos) override;
   void onWindowScrolled(double xoffset, double yoffset) override;
   void onWindowKeyChanged(gfl::Key key, int scancode, int action, int mods) override;
   void onWindowMouseButtonChanged(gfl::MouseButton button, int action, int mods,
                                   double xpos, double ypos) override;

 private:
   InputController* m_inputController = nullptr;
   glm::ivec2 m_wndSize{0, 0};
};


///////////////////

// Notifications sent to observers.

constexpr char WindowResizedEvent[] = "window_resized";
struct WindowResizedData : public sutil::ObservedEventData
{
   glm::ivec2 newSize{0, 0};
   // Difference to previous size.
   glm::ivec2 diff{0, 0};
};
