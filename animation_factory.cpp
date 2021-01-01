//
// Dec-2020, Michael Lindner
// MIT license
//
#include "animation_factory.h"
#include "animation.h"
#include "animation_tags.h"
#include "defender_models.h"
#include "map_coord_sys.h"
#include "sprite_renderer.h"
#include "texture_tags.h"


///////////////////

static Animation NullAnimation;


static Animation makeExplosion(PixDim size, SpriteRenderer* renderer)
{
   const SpriteForm form{size, Angle_t{0.f}};
   std::vector<Sprite> sprites{
      Sprite{renderer, SpriteLook{Explosion1TTag}, form},
      Sprite{renderer, SpriteLook{Explosion2TTag}, form},
      Sprite{renderer, SpriteLook{Explosion3TTag}, form},
      Sprite{renderer, SpriteLook{Explosion4TTag}, form},
   };
   std::vector<int> frames{15, 15, 15, 15};

   return Animation(sprites, frames, false);
}


static Animation makeFiringLaserTurret(PixDim size, SpriteRenderer* renderer)
{
   const SpriteForm form{size, Angle_t{0.f}};
   std::vector<Sprite> sprites{
      Sprite{renderer, SpriteLook{LtFiring1Texture}, form},
      Sprite{renderer, SpriteLook{LtFiring2Texture}, form},
   };
   std::vector<int> frames{10, 10};

   return Animation(sprites, frames, true);
}


static Animation makeFiringSonicMortar(PixDim size, SpriteRenderer* renderer)
{
   const SpriteForm form{size, Angle_t{0.f}};
   std::vector<Sprite> sprites{
      Sprite{renderer, SpriteLook{SmFiring1Texture}, form},
      Sprite{renderer, SpriteLook{SmFiring2Texture}, form},
   };
   std::vector<int> frames{10, 10};

   return Animation(sprites, frames, true);
}


///////////////////

AnimationFactory::AnimationFactory(SpriteRenderer* renderer)
: m_renderer{renderer}
{
   assert(m_renderer);

   m_factories = {
      {ExplosionATag, makeExplosion},
      {LtFiringAnimation, makeFiringLaserTurret},
      {SmFiringAnimation, makeFiringSonicMortar},
   };
}


Animation AnimationFactory::make(const std::string& tag, PixDim size)
{
   const auto pos = m_factories.find(tag);
   if (pos == m_factories.end())
      return NullAnimation;
   // Call factory function.
   return pos->second(size, m_renderer);
}
