//
// Jan-2021, Michael Lindner
// MIT license
//
#include "dashboard.h"
#include "defender_models.h"
#include "map_coord_sys.h"
#include "place_session.h"
#include "sprite.h"
#include "texture_tags.h"
#include <memory>


bool Dashboard::setup(SpriteRenderer* renderer, const MapCoordSys* cs)
{
   assert(renderer);
   assert(cs);

   m_mapCoordSys = cs;
   m_background =
      std::make_unique<Sprite>(renderer, SpriteLook{DashboardTTag}, SpriteForm{m_dim});

   constexpr NormDim buttonDim{.375f, .0625f};
   m_ltButton = std::make_unique<Sprite>(renderer, SpriteLook{LtTexture},
                                         SpriteForm{buttonDim * m_dim});
   m_smButton = std::make_unique<Sprite>(renderer, SpriteLook{SmTexture},
                                         SpriteForm{buttonDim * m_dim});

   return true;
}


void Dashboard::render(const gll::Program& shaders, const PixPos& at)
{
   m_background->render(shaders, at);

   constexpr NormDim ltPos{.075f, .0167f};
   const NormDim ltPixPos = ltPos * m_dim;
   m_ltButton->render(shaders, at + ltPixPos);

   constexpr NormDim smPos{.535f, .0167f};
   const NormDim smPixPos = smPos * m_dim;
   m_smButton->render(shaders, at + smPixPos);
}


bool Dashboard::onLeftButtonPressed(const glm::vec2& mousePosInDash)
{
   // Cancel existing placement session.
   m_commands->endPlaceSession();

   constexpr NormDim buttonDim{.375f, .0625f};
   const PixDim buttonPixDim = buttonDim * m_dim;
   // Set size of indicator to size of one field on map.
   constexpr MapDim indicatorDim{1.f, 1.f};
   const PixDim indicatorPixDim = m_mapCoordSys->toRenderCoords(indicatorDim);

   constexpr NormDim ltPos{.075f, .0167f};
   const NormDim ltPixPos = ltPos * m_dim;
   const bool isInLtButton =
      mousePosInDash.x > ltPixPos.x && mousePosInDash.x <= ltPixPos.x + buttonPixDim.x &&
      mousePosInDash.y > ltPixPos.y && mousePosInDash.y <= ltPixPos.y + buttonPixDim.y;
   if (isInLtButton)
   {
      m_commands->startPlaceSession(LtModel, LtTexture, indicatorPixDim);
      return true;
   }

   constexpr NormDim smPos{.535f, .0167f};
   const NormDim smPixPos = smPos * m_dim;
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
