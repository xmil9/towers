//
// Jan-2021, Michael Lindner
// MIT license
//
#pragma once
#include "commands.h"
#include "coords.h"
#include "sprite.h"
#include <cassert>
#include <memory>
#include <optional>

class MapCoordSys;
struct PlaceSession;
class Renderer2;


///////////////////

class Dashboard
{
 public:
   Dashboard(PixCoordi width, PixCoordi height, Commands* commands);

   bool setup(const MapCoordSys* cs);
   void render(const Renderer2& renderer, const PixPos& at);
   bool onLeftButtonPressed(const PixPos& mousePosInDash);
   bool onLeftButtonReleased(const PixPos& mousePosInDash);

 private:
   PixDim m_dim;
   Commands* m_commands = nullptr;
   const MapCoordSys* m_mapCoordSys = nullptr;
   Sprite m_background;
   Sprite m_ltButton;
   Sprite m_smButton;
};
