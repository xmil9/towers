//
// Jan-2021, Michael Lindner
// MIT license
//
#include "dashboard.h"
#include "basic_types.h"
#include "commands.h"
#include "defender_models.h"
#include "map_coord_sys.h"
#include "place_session.h"
#include "renderer2.h"
#include "sprite.h"
#include "state.h"
#include "texture_tags.h"
#include <memory>


///////////////////

static constexpr NormDim CreditsPos{.075f, .025f};
static constexpr NormDim ButtonDim{.375f, .0625f};
static constexpr NormDim LabelValueGap{.03f, 0.f};
static constexpr NormDim LaserTurretPos{.075f, .05f};
static constexpr NormDim SonarMortarPos{.535f, .05f};


///////////////////

Dashboard::Dashboard(PixCoordi width, PixCoordi height, Commands* commands, State* state)
: m_dim{width, height}, m_commands{commands}, m_state{state},
  m_background{SpriteLook{DashboardTTag}, SpriteForm{m_dim}}
{
   assert(m_commands);
   assert(m_state);
}


bool Dashboard::setup(const MapCoordSys* cs)
{
   assert(cs);
   m_mapCoordSys = cs;

   Sprite buttonBkg{SpriteLook{ButtonBackgroundTTag}, SpriteForm{ButtonDim * m_dim}};

   m_ltButton.setup(
      buttonBkg, Sprite{SpriteLook{LtTexture}, SpriteForm{ButtonDim * m_dim}},
      [this]() { return m_state->canAffordDefender(LtModel); }, LaserTurretPos * m_dim,
      ButtonDim * m_dim);
   m_smButton.setup(
      buttonBkg, Sprite{SpriteLook{SmTexture}, SpriteForm{ButtonDim * m_dim}},
      [this]() { return m_state->canAffordDefender(SmModel); }, SonarMortarPos * m_dim,
      ButtonDim * m_dim);

   return true;
}


void Dashboard::render(Renderer2& renderer, const PixPos& at)
{
   renderer.renderSprite(m_background, at);

   constexpr float TextScale = .8f * UiScale(1.f);
   constexpr Color TextColor{.3f, .3f, .3f};

   const std::string label{"Credits:"};
   const PixDim labelPixPos = CreditsPos * m_dim;
   renderer.renderText(label, at + labelPixPos, TextScale, TextColor);
   const PixDim labelWidth = {renderer.measureText(label, TextScale).x, 0.f};

   const PixDim gap = LabelValueGap * m_dim;
   const PixDim creditsPixPos = labelPixPos + gap + labelWidth;
   renderer.renderText(std::to_string(m_state->credits()), at + creditsPixPos, TextScale,
                       TextColor);

   m_ltButton.render(renderer, at);
   m_smButton.render(renderer, at);
}


bool Dashboard::onLeftButtonPressed(const glm::vec2& mousePosInDash)
{
   // Cancel existing placement session.
   m_commands->endPlaceSession();

   const PixDim buttonPixDim = ButtonDim * m_dim;
   // Set size of indicator to size of one field on map.
   constexpr MapDim indicatorDim{1.f, 1.f};
   const PixDim indicatorPixDim = m_mapCoordSys->toRenderCoords(indicatorDim);

   if (m_ltButton.isHit(mousePosInDash) && m_ltButton.isEnabled())
   {
      m_commands->startPlaceSession(LtModel, LtTexture, indicatorPixDim);
      return true;
   }

   if (m_smButton.isHit(mousePosInDash) && m_smButton.isEnabled())
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
