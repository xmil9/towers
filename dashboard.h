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

struct PlaceSession;


///////////////////

class Dashboard
{
 public:
   Dashboard(PixCoordi width, PixCoordi height, Commands* commands);

   bool setup(SpriteRenderer* renderer);
   void render(const gll::Program& shaders, const PixPos& at);
   bool onLeftButtonPressed(const PixPos& mousePosInDash);
   bool onLeftButtonReleased(const PixPos& mousePosInDash);

 private:
   PixDim m_dim;
   Commands* m_commands = nullptr;
   std::unique_ptr<Sprite> m_background;
   std::unique_ptr<Sprite> m_ltButton;
   std::unique_ptr<Sprite> m_smButton;
};


inline Dashboard::Dashboard(PixCoordi width, PixCoordi height, Commands* commands)
: m_dim{width, height}, m_commands{commands}
{
   assert(m_commands);
}
