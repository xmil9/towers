//
// Jan-2021, Michael Lindner
// MIT license
//
#pragma once
#include "button.h"
#include "coords.h"
#include "label.h"
#include "sge_sprite.h"

struct Commands;
class MapCoordSys;
struct PlaceSession;
class Renderer2;
struct State;


///////////////////

class Dashboard
{
 public:
   Dashboard(sge::PixCoordi width, sge::PixCoordi height, Commands* commands, State* state);

   bool setup(const Renderer2& renderer, const MapCoordSys* cs);
   void render(Renderer2& renderer, const sge::PixPos& at);
   bool onLeftButtonPressed(const sge::PixPos& mousePosInDash);
   bool onLeftButtonReleased(const sge::PixPos& mousePosInDash);

 private:
   void setupCreditsElements(const Renderer2& renderer);
   void setupDefenderElements();
   void setupGameflowElements();

   sge::PixPos toPix(sge::NormPos npos) const { return npos * m_dim; }
   sge::PixCoord toVertPix(sge::NormCoord ny) const { return ny * m_dim.y; }
   sge::PixCoord toHorzPix(sge::NormCoord nx) const { return nx * m_dim.x; }

 private:
   sge::PixDim m_dim;
   Commands* m_commands = nullptr;
   State* m_state = nullptr;
   const MapCoordSys* m_mapCoordSys = nullptr;
   sge::Sprite m_background;
   Label m_creditsLabel;
   Label m_creditsValue;
   Button m_ltButton;
   Label m_ltStats;
   Button m_smButton;
   Label m_smStats;
   Button m_startButton;
};
