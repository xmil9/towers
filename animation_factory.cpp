//
// Dec-2020, Michael Lindner
// MIT license
//
#include "animation_factory.h"
#include "animation_tags.h"
#include "defender_models.h"
#include "map_coord_sys.h"
#include "texture_tags.h"
#include "sge_animation.h"


///////////////////

static sge::Animation NullAnimation;


static sge::Animation makeExplosion(sge::PixDim size)
{
   const sge::SpriteForm form{size, sge::Angle{0.f}};
   std::vector<sge::Sprite> sprites{
      sge::Sprite{sge::SpriteLook{Explosion1TTag}, form},
      sge::Sprite{sge::SpriteLook{Explosion2TTag}, form},
      sge::Sprite{sge::SpriteLook{Explosion3TTag}, form},
      sge::Sprite{sge::SpriteLook{Explosion4TTag}, form},
   };
   std::vector<int> frames{15, 15, 15, 15};

   return sge::Animation(sprites, frames, false);
}


static sge::Animation makeFiringLaserTurret(sge::PixDim size)
{
   const sge::SpriteForm form{size, sge::Angle{0.f}};
   std::vector<sge::Sprite> sprites{
      sge::Sprite{sge::SpriteLook{LtFiring1Texture}, form},
      sge::Sprite{sge::SpriteLook{LtFiring2Texture}, form},
   };
   std::vector<int> frames{10, 10};

   return sge::Animation(sprites, frames, true);
}


static sge::Animation makeFiringSonicMortar(sge::PixDim size)
{
   const sge::SpriteForm form{size, sge::Angle{0.f}};
   std::vector<sge::Sprite> sprites{
      sge::Sprite{sge::SpriteLook{SmFiring1Texture}, form},
      sge::Sprite{sge::SpriteLook{SmFiring2Texture}, form},
   };
   std::vector<int> frames{10, 10};

   return sge::Animation(sprites, frames, true);
}


///////////////////

AnimationFactory::AnimationFactory()
{
   m_factories = {
      {ExplosionATag, makeExplosion},
      {LtFiringAnimation, makeFiringLaserTurret},
      {SmFiringAnimation, makeFiringSonicMortar},
   };
}


sge::Animation AnimationFactory::make(const std::string& tag, sge::PixDim size)
{
   const auto pos = m_factories.find(tag);
   if (pos == m_factories.end())
      return NullAnimation;
   // Call factory function.
   return pos->second(size);
}
