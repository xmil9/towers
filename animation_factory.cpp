//
// Dec-2020, Michael Lindner
// MIT license
//
#include "animation_factory.h"
#include "animation.h"
#include "animation_tags.h"
#include "map_coord_sys.h"
#include "sprite_renderer.h"
#include "texture_tags.h"


///////////////////

static Animation NullAnimation;


static Animation makeExplosion(PixDim size, SpriteRenderer* renderer,
                               const MapCoordSys* cs)
{
   const SpriteForm form{size, Angle_t{0.f}};
   std::vector<Sprite> sprites{
      Sprite{renderer, SpriteLook{Explosion1TTag}, form},
      Sprite{renderer, SpriteLook{Explosion2TTag}, form},
      Sprite{renderer, SpriteLook{Explosion3TTag}, form},
      Sprite{renderer, SpriteLook{Explosion4TTag}, form},
   };
   std::vector<int> frames{15, 15, 15, 15};

   return Animation(sprites, frames, false, cs);
}


static Animation makeFiringDefender(PixDim size, SpriteRenderer* renderer,
                                    const MapCoordSys* cs)
{
   const SpriteForm form{size, Angle_t{0.f}};
   std::vector<Sprite> sprites{
      Sprite{renderer, SpriteLook{FiringDefender1TTag}, form},
      Sprite{renderer, SpriteLook{FiringDefender2TTag}, form},
   };
   std::vector<int> frames{10, 10};

   return Animation(sprites, frames, true, cs);
}


///////////////////

AnimationFactory::AnimationFactory(SpriteRenderer* renderer, const MapCoordSys* cs)
: m_renderer{renderer}, m_coordSys{cs}
{
   assert(m_renderer);
   assert(m_coordSys);

   m_factories = {
      {ExplosionATag, makeExplosion},
      {FiringDefenderATag, makeFiringDefender},
   };
}


Animation AnimationFactory::make(const std::string& tag, PixDim size)
{
   const auto pos = m_factories.find(tag);
   if (pos == m_factories.end())
      return NullAnimation;
   // Call factory function.
   return pos->second(size, m_renderer, m_coordSys);
}
