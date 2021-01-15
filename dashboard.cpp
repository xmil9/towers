//
// Jan-2021, Michael Lindner
// MIT license
//
#include "dashboard.h"
#include "defender_models.h"
#include "map_coord_sys.h"
#include "place_session.h"
#include "renderer2.h"
#include "sprite.h"
#include "texture_tags.h"
#include <memory>


///////////////////

static constexpr NormDim CreditsPos{.075f, .025f};
static constexpr NormDim ButtonDim{.375f, .0625f};
static constexpr NormDim LaserTurretPos{.075f, .05f};
static constexpr NormDim SonarMortarPos{.535f, .05f};


///////////////////

Dashboard::Dashboard(PixCoordi width, PixCoordi height, Commands* commands)
: m_dim{width, height}, m_commands{commands}, m_background{SpriteLook{DashboardTTag},
                                                           SpriteForm{m_dim}},
  m_buttonBackground{SpriteLook{ButtonBackgroundTTag}, SpriteForm{ButtonDim * m_dim}},
  m_ltButton{SpriteLook{LtTexture}, SpriteForm{ButtonDim * m_dim}},
  m_smButton{SpriteLook{SmTexture}, SpriteForm{ButtonDim * m_dim}}

{
   assert(m_commands);
}


bool Dashboard::setup(const MapCoordSys* cs)
{
   assert(cs);
   m_mapCoordSys = cs;
   return true;
}


void Dashboard::render(Renderer2& renderer, const PixPos& at)
{
   renderer.renderSprite(m_background, at);

   renderer.beginTextRendering();
   const NormDim creditsPixPos = CreditsPos * m_dim;
   renderer.renderText("Credits:", at + creditsPixPos, .8f, {.3f, .3f, .3f});

   renderer.beginSpriteRendering();
   const NormDim ltPixPos = LaserTurretPos * m_dim;
   renderer.renderSprite(m_buttonBackground, at + ltPixPos);
   renderer.renderSprite(m_ltButton, at + ltPixPos);

   const NormDim smPixPos = SonarMortarPos * m_dim;
   renderer.renderSprite(m_buttonBackground, at + smPixPos);
   renderer.renderSprite(m_smButton, at + smPixPos);
}


bool Dashboard::onLeftButtonPressed(const glm::vec2& mousePosInDash)
{
   // Cancel existing placement session.
   m_commands->endPlaceSession();

   const PixDim buttonPixDim = ButtonDim * m_dim;
   // Set size of indicator to size of one field on map.
   constexpr MapDim indicatorDim{1.f, 1.f};
   const PixDim indicatorPixDim = m_mapCoordSys->toRenderCoords(indicatorDim);

   const NormDim ltPixPos = LaserTurretPos * m_dim;
   const bool isInLtButton =
      mousePosInDash.x > ltPixPos.x && mousePosInDash.x <= ltPixPos.x + buttonPixDim.x &&
      mousePosInDash.y > ltPixPos.y && mousePosInDash.y <= ltPixPos.y + buttonPixDim.y;
   if (isInLtButton)
   {
      m_commands->startPlaceSession(LtModel, LtTexture, indicatorPixDim);
      return true;
   }

   const NormDim smPixPos = SonarMortarPos * m_dim;
   const bool isInSmButton =
      mousePosInDash.x > smPixPos.x && mousePosInDash.x <= smPixPos.x + buttonPixDim.x &&
      mousePosInDash.y > smPixPos.y && mousePosInDash.y <= smPixPos.y + buttonPixDim.y;
   if (isInSmButton)
   {
      m_commands->startPlaceSession(SmModel, SmTexture, indicatorPixDim);
      return true;
   }

   return false;
}


bool Dashboard::onLeftButtonReleased(const glm::vec2& /*mousePosInDash*/)
{
   return false;
}
