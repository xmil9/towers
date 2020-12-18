//
// Doc-2020, Michael Lindner
// MIT license
//
#include "animation_factory.h"
#include "animation.h"
#include "map_coord_sys.h"
#include "sprite_renderer.h"


///////////////////

static Animation NullAnimation;


static Animation makeExplosion(PixDim size, SpriteRenderer* renderer,
                               const MapCoordSys* cs)
{
   const SpriteForm form{size, Angle_t{0.f}};
   std::vector<Sprite> sprites{
      Sprite{renderer, SpriteLook{"explosion1"}, form},
      Sprite{renderer, SpriteLook{"explosion2"}, form},
      Sprite{renderer, SpriteLook{"explosion3"}, form},
      Sprite{renderer, SpriteLook{"explosion4"}, form},
   };
   std::vector<int> frames{15, 15, 15, 15};

   return Animation(sprites, frames, false, cs);
}


static Animation makeFiringDefender(PixDim size, SpriteRenderer* renderer,
                                    const MapCoordSys* cs)
{
   const SpriteForm form{size, Angle_t{0.f}};
   std::vector<Sprite> sprites{
      Sprite{renderer, SpriteLook{"defender_firing1"}, form},
      Sprite{renderer, SpriteLook{"defender_firing2"}, form},
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
      {"explosion", makeExplosion},
      {"defender_firing", makeFiringDefender},
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
