//
// Jan-2021, Michael Lindner
// MIT license
//
#pragma once
#include "coords.h"
#include "sprite.h"

struct Commands;
class MapCoordSys;
struct PlaceSession;
class Renderer2;
struct State;


///////////////////

class Dashboard
{
 public:
   Dashboard(PixCoordi width, PixCoordi height, Commands* commands, State* state);

   bool setup(const MapCoordSys* cs);
   void render(Renderer2& renderer, const PixPos& at);
   bool onLeftButtonPressed(const PixPos& mousePosInDash);
   bool onLeftButtonReleased(const PixPos& mousePosInDash);

 private:
   PixDim m_dim;
   Commands* m_commands = nullptr;
   State* m_state = nullptr;
   const MapCoordSys* m_mapCoordSys = nullptr;
   Sprite m_background;
   Sprite m_buttonBackground;
   Sprite m_ltButton;
   Sprite m_smButton;
};
