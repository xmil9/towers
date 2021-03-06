//
// Jan-2021, Michael Lindner
// MIT license
//
#include "dashboard.h"
#include "commands.h"
#include "defender_models.h"
#include "laser_turret.h"
#include "map_coord_sys.h"
#include "place_session.h"
#include "sonic_mortar.h"
#include "state.h"
#include "texture_tags.h"
#include "ui_scale.h"
#include "spiel/renderer2.h"
#include "spiel/sprite.h"
#include "essentutils/string_util.h"
#include <memory>


namespace
{

///////////////////

constexpr sp::Color TextColor{.3f, .3f, .3f};

constexpr sp::NormDim CreditsPos{.075f, .025f};
constexpr sp::NormDim CreditsValueGap{.03f, 0.f};
const std::string CreditsText{"Credits:"};
constexpr float CreditsTextScale = .8f * UiScale(1.f);
constexpr sp::Color CreditsTextColor = TextColor;

constexpr sp::NormDim LaserTurretPos{.075f, .05f};
constexpr sp::NormDim SonarMortarPos{.535f, .05f};
constexpr sp::NormDim DefenderButtonDim{.375f, .0625f};
constexpr sp::NormCoord StatsVertGap = .014f;
constexpr float TextScaleForCost = .7f * UiScale(1.f);
constexpr float TextScaleForAllStats = .5f * UiScale(1.f);
constexpr sp::Color StatsTextColor = TextColor;

constexpr sp::NormDim StartPos{.075f, .145f};
constexpr sp::NormDim FlowButtonDim = .5f * DefenderButtonDim;


///////////////////

std::string FormatFloat(float f)
{
   constexpr float precision = 100.f;
   const float rounded = static_cast<float>(static_cast<int>(precision * f)) / precision;

   std::string s = esl::trimRight(std::to_string(rounded), '0');
   if (esl::endsWith(s, "."))
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

Dashboard::Dashboard(sp::PixCoordi width, sp::PixCoordi height, Commands* commands,
                     State* state)
: m_dim{width, height}, m_commands{commands}, m_state{state},
  m_background{sp::SpriteLook{DashboardTexture}, sp::SpriteForm{m_dim}},
  m_creditsLabel{CreditsTextScale, CreditsTextColor}, m_creditsValue{CreditsTextScale,
                                                                     CreditsTextColor},
  m_ltStats{TextScaleForCost, StatsTextColor}, m_smStats{TextScaleForCost, StatsTextColor}
{
   assert(m_commands);
   assert(m_state);
}


bool Dashboard::setup(const sp::Renderer2& renderer, const MapCoordSys* cs)
{
   assert(cs);
   m_mapCoordSys = cs;

   setupCreditsElements(renderer);
   setupDefenderElements();
   setupGameflowElements();

   return true;
}


void Dashboard::render(sp::Renderer2& renderer, const sp::PixPos& at)
{
   renderer.renderSprite(m_background, at);
   m_creditsLabel.render(renderer, at);
   m_creditsValue.render(renderer, at);
   m_ltButton.render(renderer, at);
   m_ltStats.render(renderer, at);
   m_smButton.render(renderer, at);
   m_smStats.render(renderer, at);
   m_startButton.render(renderer, at);
}


bool Dashboard::onLeftButtonPressed(const glm::vec2& mousePosInDash)
{
   // Cancel existing placement session.
   m_commands->endPlaceSession();

   // Set size of indicator to size of one field on map.
   constexpr sp::MapDim indicatorDim{1.f, 1.f};
   const sp::PixDim indicatorsPixDim = m_mapCoordSys->toPix(indicatorDim);

   if (m_ltButton.isHit(mousePosInDash) && m_ltButton.isEnabled())
   {
      m_commands->startPlaceSession(LtModel, LtTexture, indicatorsPixDim);
      return true;
   }

   if (m_smButton.isHit(mousePosInDash) && m_smButton.isEnabled())
   {
      m_commands->startPlaceSession(SmModel, SmTexture, indicatorsPixDim);
      return true;
   }

   if (m_startButton.isHit(mousePosInDash) && m_startButton.isEnabled())
   {
      const sp::PixDim buttonPixDim = toPix(FlowButtonDim);
      sp::SpriteForm contentForm{buttonPixDim};

      if (m_state->isPaused())
      {
         m_commands->startAttack();
         m_startButton.setContent(sp::Sprite{sp::SpriteLook{PauseTexture}, contentForm});
      }
      else
      {
         m_commands->pauseAttack();
         m_startButton.setContent(sp::Sprite{sp::SpriteLook{StartTexture}, contentForm});
      }
      return true;
   }

   return false;
}


bool Dashboard::onLeftButtonReleased(const glm::vec2& /*mousePosInDash*/)
{
   return false;
}


void Dashboard::setupCreditsElements(const sp::Renderer2& renderer)
{
   const sp::PixDim ceditsLabelDim = renderer.measureText(CreditsText, CreditsTextScale);
   m_creditsLabel.setup([]() { return CreditsText; }, CreditsPos * m_dim, ceditsLabelDim);

   const sp::PixDim gap = toPix(CreditsValueGap);
   const sp::PixDim creditsValPos =
      m_creditsLabel.position() + sp::PixDim{ceditsLabelDim.x, 0.f} + gap;
   const sp::PixDim ceditsValDim{0.f, ceditsLabelDim.y};
   m_creditsValue.setup([this]() { return std::to_string(m_state->credits()); },
                        creditsValPos, ceditsValDim);
}


void Dashboard::setupDefenderElements()
{
   const sp::PixDim buttonPixDim = toPix(DefenderButtonDim);
   sp::Sprite buttonBkg{sp::SpriteLook{ButtonBackgroundTexture},
                        sp::SpriteForm{buttonPixDim}};
   sp::SpriteForm contentForm{buttonPixDim};

   const sp::PixPos ltPixPos = toPix(LaserTurretPos);
   m_ltButton.setup(
      buttonBkg, sp::Sprite{sp::SpriteLook{LtTexture}, contentForm},
      [this]() { return m_state->canAffordDefender(LtModel); }, ltPixPos, buttonPixDim);

   const sp::PixVec statsOffset{0.f, buttonPixDim.y + toVertPix(StatsVertGap)};
   const sp::PixPos ltStatsPixPos = ltPixPos + statsOffset;
   m_ltStats.setup([]() { return FormatDefenderStats(LaserTurret::defaultAttributes()); },
                   ltStatsPixPos, {});

   const sp::PixPos smPixPos = toPix(SonarMortarPos);
   m_smButton.setup(
      buttonBkg, sp::Sprite{sp::SpriteLook{SmTexture}, contentForm},
      [this]() { return m_state->canAffordDefender(SmModel); }, smPixPos, buttonPixDim);

   const sp::PixPos smStatsPixPos = smPixPos + statsOffset;
   m_smStats.setup([]() { return FormatDefenderStats(SonicMortar::defaultAttributes()); },
                   smStatsPixPos, {});
}


void Dashboard::setupGameflowElements()
{
   const sp::PixDim buttonPixDim = toPix(FlowButtonDim);
   sp::Sprite buttonBkg{sp::SpriteLook{ButtonBackgroundTexture},
                        sp::SpriteForm{buttonPixDim}};
   sp::SpriteForm contentForm{buttonPixDim};

   const sp::PixPos startPixPos = toPix(StartPos);
   m_startButton.setup(
      buttonBkg, sp::Sprite{sp::SpriteLook{StartTexture}, contentForm},
      [this]() { return true; }, startPixPos, buttonPixDim);
}
