//
// Jan-2021, Michael Lindner
// MIT license
//
#pragma once
#include "button.h"
#include "coords.h"
#include "label.h"
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

   bool setup(const Renderer2& renderer, const MapCoordSys* cs);
   void render(Renderer2& renderer, const PixPos& at);
   bool onLeftButtonPressed(const PixPos& mousePosInDash);
   bool onLeftButtonReleased(const PixPos& mousePosInDash);

 private:
   void setupCreditsElements(const Renderer2& renderer);
   void setupDefenderElements();

 private:
   PixDim m_dim;
   Commands* m_commands = nullptr;
   State* m_state = nullptr;
   const MapCoordSys* m_mapCoordSys = nullptr;
   Sprite m_background;
   Label m_creditsLabel;
   Label m_creditsValue;
   Button m_ltButton;
   Label m_ltStats;
   Button m_smButton;
   Label m_smStats;
};
