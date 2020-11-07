//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "ui_controller.h"
#include "observed.h"
#include <glm/vec2.hpp>


///////////////////

class UiState : public UiController, public Observed<UiState>
{
 private:
   // UiController overrides.
   void onWindowResized(int width, int height) override;

private:
   glm::ivec2 m_wndSize;
};


///////////////////

// Notifications sent to UI state observers.

constexpr char WindowResizedMsg[] = "window_resized";
struct WindowResizedMsgData : public Observed<UiState>::MsgData
{
   glm::ivec2 size;
};
