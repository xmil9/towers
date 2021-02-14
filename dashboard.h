//
// Jan-2021, Michael Lindner
// MIT license
//
#pragma once
#include "sge_button.h"
#include "sge_coords.h"
#include "sge_label.h"
#include "sge_sprite.h"

struct Commands;
class MapCoordSys;
struct PlaceSession;
struct State;
namespace sge
{
class Renderer2;
}


///////////////////

class Dashboard
{
 public:
   Dashboard(sge::PixCoordi width, sge::PixCoordi height, Commands* commands, State* state);

   bool setup(const sge::Renderer2& renderer, const MapCoordSys* cs);
   void render(sge::Renderer2& renderer, const sge::PixPos& at);
   bool onLeftButtonPressed(const sge::PixPos& mousePosInDash);
   bool onLeftButtonReleased(const sge::PixPos& mousePosInDash);

 private:
   void setupCreditsElements(const sge::Renderer2& renderer);
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
   sge::Label m_creditsLabel;
   sge::Label m_creditsValue;
   sge::Button m_ltButton;
   sge::Label m_ltStats;
   sge::Button m_smButton;
   sge::Label m_smStats;
   sge::Button m_startButton;
};
