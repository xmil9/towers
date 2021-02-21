//
// Dec-2020, Michael Lindner
// MIT license
//
#include "animation_factory.h"
#include "animation_tags.h"
#include "defender_models.h"
#include "map_coord_sys.h"
#include "texture_tags.h"
#include "spiel/animation.h"


///////////////////

static sp::Animation NullAnimation;


static sp::Animation makeExplosion(sp::PixDim size)
{
   const sp::SpriteForm form{size, sp::Angle{0.f}};
   std::vector<sp::Sprite> sprites{
      sp::Sprite{sp::SpriteLook{Explosion1Texture}, form},
      sp::Sprite{sp::SpriteLook{Explosion2Texture}, form},
      sp::Sprite{sp::SpriteLook{Explosion3Texture}, form},
      sp::Sprite{sp::SpriteLook{Explosion4Texture}, form},
   };
   std::vector<int> frames{15, 15, 15, 15};

   return sp::Animation(sprites, frames, false);
}


static sp::Animation makeFiringLaserTurret(sp::PixDim size)
{
   const sp::SpriteForm form{size, sp::Angle{0.f}};
   std::vector<sp::Sprite> sprites{
      sp::Sprite{sp::SpriteLook{LtFiring1Texture}, form},
      sp::Sprite{sp::SpriteLook{LtFiring2Texture}, form},
   };
   std::vector<int> frames{10, 10};

   return sp::Animation(sprites, frames, true);
}


static sp::Animation makeFiringSonicMortar(sp::PixDim size)
{
   const sp::SpriteForm form{size, sp::Angle{0.f}};
   std::vector<sp::Sprite> sprites{
      sp::Sprite{sp::SpriteLook{SmFiring1Texture}, form},
      sp::Sprite{sp::SpriteLook{SmFiring2Texture}, form},
   };
   std::vector<int> frames{10, 10};

   return sp::Animation(sprites, frames, true);
}


///////////////////

AnimationFactory::AnimationFactory()
{
   m_factories = {
      {ExplosionAnimation, makeExplosion},
      {LtFiringAnimation, makeFiringLaserTurret},
      {SmFiringAnimation, makeFiringSonicMortar},
   };
}


sp::Animation AnimationFactory::make(const std::string& tag, sp::PixDim size)
{
   const auto pos = m_factories.find(tag);
   if (pos == m_factories.end())
      return NullAnimation;
   // Call factory function.
   return pos->second(size);
}
