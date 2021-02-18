//
// Jan-2021, Michael Lindner
// MIT license
//
#pragma once
#include "spiel/button.h"
#include "spiel/coords.h"
#include "spiel/label.h"
#include "spiel/sprite.h"

struct Commands;
class MapCoordSys;
struct PlaceSession;
struct State;
namespace sp
{
class Renderer2;
}


///////////////////

class Dashboard
{
 public:
   Dashboard(sp::PixCoordi width, sp::PixCoordi height, Commands* commands, State* state);

   bool setup(const sp::Renderer2& renderer, const MapCoordSys* cs);
   void render(sp::Renderer2& renderer, const sp::PixPos& at);
   bool onLeftButtonPressed(const sp::PixPos& mousePosInDash);
   bool onLeftButtonReleased(const sp::PixPos& mousePosInDash);

 private:
   void setupCreditsElements(const sp::Renderer2& renderer);
   void setupDefenderElements();
   void setupGameflowElements();

   sp::PixPos toPix(sp::NormPos npos) const { return npos * m_dim; }
   sp::PixCoord toVertPix(sp::NormCoord ny) const { return ny * m_dim.y; }
   sp::PixCoord toHorzPix(sp::NormCoord nx) const { return nx * m_dim.x; }

 private:
   sp::PixDim m_dim;
   Commands* m_commands = nullptr;
   State* m_state = nullptr;
   const MapCoordSys* m_mapCoordSys = nullptr;
   sp::Sprite m_background;
   sp::Label m_creditsLabel;
   sp::Label m_creditsValue;
   sp::Button m_ltButton;
   sp::Label m_ltStats;
   sp::Button m_smButton;
   sp::Label m_smStats;
   sp::Button m_startButton;
};
