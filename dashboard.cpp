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

static std::string CreditsText{"Credits:"};
static constexpr float TextScale = .8f * UiScale(1.f);
static constexpr Color TextColor{.3f, .3f, .3f};


///////////////////

Dashboard::Dashboard(PixCoordi width, PixCoordi height, Commands* commands, State* state)
: m_dim{width, height}, m_commands{commands}, m_state{state},
  m_background{SpriteLook{DashboardTTag}, SpriteForm{m_dim}},
  m_creditsLabel{TextScale, TextColor}, m_creditsValue{TextScale, TextColor}
{
   assert(m_commands);
   assert(m_state);
}


bool Dashboard::setup(const Renderer2& renderer, const MapCoordSys* cs)
{
   assert(cs);
   m_mapCoordSys = cs;

   setupCreditsElements(renderer);
   setupDefenderElements();

   return true;
}


void Dashboard::render(Renderer2& renderer, const PixPos& at)
{
   renderer.renderSprite(m_background, at);
   m_creditsLabel.render(renderer, at);
   m_creditsValue.render(renderer, at);
   m_ltButton.render(renderer, at);
   m_smButton.render(renderer, at);
}


bool Dashboard::onLeftButtonPressed(const glm::vec2& mousePosInDash)
{
   // Cancel existing placement session.
   m_commands->endPlaceSession();

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


void Dashboard::setupCreditsElements(const Renderer2& renderer)
{
   const PixDim ceditsLabelDim = renderer.measureText(CreditsText, TextScale);
   m_creditsLabel.setup([]() { return CreditsText; }, CreditsPos * m_dim, ceditsLabelDim);

   const PixDim gap = LabelValueGap * m_dim;
   const PixDim creditsValPos =
      m_creditsLabel.position() + PixDim{ceditsLabelDim.x, 0.f} + gap;
   const PixDim ceditsValDim{100.f, ceditsLabelDim.y};
   m_creditsValue.setup([this]() { return std::to_string(m_state->credits()); }, creditsValPos,
                        ceditsValDim);
}


void Dashboard::setupDefenderElements()
{
   const PixDim buttonPixDim = ButtonDim * m_dim;
   Sprite buttonBkg{SpriteLook{ButtonBackgroundTTag}, SpriteForm{buttonPixDim}};
   SpriteForm contentForm{buttonPixDim};

   m_ltButton.setup(
      buttonBkg, Sprite{SpriteLook{LtTexture}, contentForm},
      [this]() { return m_state->canAffordDefender(LtModel); }, LaserTurretPos * m_dim,
      buttonPixDim);
   m_smButton.setup(
      buttonBkg, Sprite{SpriteLook{SmTexture}, contentForm},
      [this]() { return m_state->canAffordDefender(SmModel); }, SonarMortarPos * m_dim,
      buttonPixDim);
}
