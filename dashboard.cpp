//
// Jan-2021, Michael Lindner
// MIT license
//
#include "dashboard.h"
#include "basic_types.h"
#include "commands.h"
#include "defender_models.h"
#include "laser_turret.h"
#include "map_coord_sys.h"
#include "place_session.h"
#include "renderer2.h"
#include "sonic_mortar.h"
#include "sprite.h"
#include "state.h"
#include "texture_tags.h"
#include "essentutils/string_util.h"
#include <memory>


namespace
{

///////////////////

constexpr Color TextColor{.3f, .3f, .3f};

constexpr NormDim CreditsPos{.075f, .025f};
constexpr NormDim CreditsValueGap{.03f, 0.f};
const std::string CreditsText{"Credits:"};
constexpr float CreditsTextScale = .8f * UiScale(1.f);
constexpr Color CreditsTextColor = TextColor;

constexpr NormDim LaserTurretPos{.075f, .05f};
constexpr NormDim SonarMortarPos{.535f, .05f};
constexpr NormDim DefenderButtonDim{.375f, .0625f};
constexpr NormCoord StatsVertGap = .014f;
constexpr float TextScaleForCost = .7f * UiScale(1.f);
constexpr float TextScaleForAllStats = .5f * UiScale(1.f);
constexpr Color StatsTextColor = TextColor;

constexpr NormDim StartPos{.075f, .145f};
constexpr NormDim PausePos{.3f, .145f};
constexpr NormDim FlowButtonDim = .5f * DefenderButtonDim;


///////////////////

std::string FormatFloat(float f)
{
   constexpr float precision = 100.f;
   const float rounded = static_cast<float>(static_cast<int>(precision * f)) / precision;

   std::string s = sutil::trimRight(std::to_string(rounded), '0');
   if (sutil::endsWith(s, "."))
      s += "0";
   return s;
}


template <typename Attribs> std::string FormatAllDefenderStats(const Attribs& attribs)
{
   std::string stats;
   stats += "C:";
   stats += std::to_string(attribs.cost);
   stats += " D:";
   stats += std::to_string(attribs.damage);
   stats += " R:";
   stats += FormatFloat(attribs.range);
   return stats;
}


template <typename Attribs> std::string FormatDefenderCost(const Attribs& attribs)
{
   std::string stats;
   stats += "Cost:";
   stats += std::to_string(attribs.cost);
   return stats;
}


template <typename Attribs> std::string FormatDefenderStats(const Attribs& attribs)
{
   return FormatDefenderCost(attribs);
}

} // namespace


///////////////////

Dashboard::Dashboard(PixCoordi width, PixCoordi height, Commands* commands, State* state)
: m_dim{width, height}, m_commands{commands}, m_state{state},
  m_background{SpriteLook{DashboardTTag}, SpriteForm{m_dim}},
  m_creditsLabel{CreditsTextScale, CreditsTextColor}, m_creditsValue{CreditsTextScale,
                                                                     CreditsTextColor},
  m_ltStats{TextScaleForCost, StatsTextColor}, m_smStats{TextScaleForCost, StatsTextColor}
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
   setupGameflowElements();

   return true;
}


void Dashboard::render(Renderer2& renderer, const PixPos& at)
{
   renderer.renderSprite(m_background, at);
   m_creditsLabel.render(renderer, at);
   m_creditsValue.render(renderer, at);
   m_ltButton.render(renderer, at);
   m_ltStats.render(renderer, at);
   m_smButton.render(renderer, at);
   m_smStats.render(renderer, at);
   m_startButton.render(renderer, at);
   m_pauseButton.render(renderer, at);
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

   if (m_startButton.isHit(mousePosInDash) && m_startButton.isEnabled())
   {
      m_commands->startAttack();
      return true;
   }

   if (m_pauseButton.isHit(mousePosInDash) && m_pauseButton.isEnabled())
   {
      m_commands->pauseAttack();
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
   const PixDim ceditsLabelDim = renderer.measureText(CreditsText, CreditsTextScale);
   m_creditsLabel.setup([]() { return CreditsText; }, CreditsPos * m_dim, ceditsLabelDim);

   const PixDim gap = toPix(CreditsValueGap);
   const PixDim creditsValPos =
      m_creditsLabel.position() + PixDim{ceditsLabelDim.x, 0.f} + gap;
   const PixDim ceditsValDim{0.f, ceditsLabelDim.y};
   m_creditsValue.setup([this]() { return std::to_string(m_state->credits()); },
                        creditsValPos, ceditsValDim);
}


void Dashboard::setupDefenderElements()
{
   const PixDim buttonPixDim = toPix(DefenderButtonDim);
   Sprite buttonBkg{SpriteLook{ButtonBackgroundTTag}, SpriteForm{buttonPixDim}};
   SpriteForm contentForm{buttonPixDim};

   const PixPos ltPixPos = toPix(LaserTurretPos);
   m_ltButton.setup(
      buttonBkg, Sprite{SpriteLook{LtTexture}, contentForm},
      [this]() { return m_state->canAffordDefender(LtModel); }, ltPixPos, buttonPixDim);

   const PixVec statsOffset{0.f, buttonPixDim.y + toVertPix(StatsVertGap)};
   const PixPos ltStatsPixPos = ltPixPos + statsOffset;
   m_ltStats.setup([]() { return FormatDefenderStats(LaserTurret::defaultAttributes()); },
                   ltStatsPixPos, {});

   const PixPos smPixPos = toPix(SonarMortarPos);
   m_smButton.setup(
      buttonBkg, Sprite{SpriteLook{SmTexture}, contentForm},
      [this]() { return m_state->canAffordDefender(SmModel); }, smPixPos, buttonPixDim);

   const PixPos smStatsPixPos = smPixPos + statsOffset;
   m_smStats.setup([]() { return FormatDefenderStats(SonicMortar::defaultAttributes()); },
                   smStatsPixPos, {});
}


void Dashboard::setupGameflowElements()
{
   const PixDim buttonPixDim = toPix(FlowButtonDim);
   Sprite buttonBkg{SpriteLook{ButtonBackgroundTTag}, SpriteForm{buttonPixDim}};
   SpriteForm contentForm{buttonPixDim};

   const PixPos startPixPos = toPix(StartPos);
   m_startButton.setup(
      buttonBkg, Sprite{SpriteLook{StartTTag}, contentForm},
      [this]() { return m_state->canStartAttack(); }, startPixPos, buttonPixDim);

   const PixPos pausePixPos = toPix(PausePos);
   m_pauseButton.setup(
      buttonBkg, Sprite{SpriteLook{PauseTTag}, contentForm},
      [this]() { return m_state->canPauseAttack(); }, pausePixPos, buttonPixDim);
}
