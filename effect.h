//
// Dec-2020, Michael Lindner
// MIT license
//
#pragma once
#include "animation.h"

namespace gll
{
class Program;
}


///////////////////

// An animation at a position.
class Effect
{
 public:
   explicit Effect(Animation animation, const MapCoordSys* cs, NormPos center = {});

   void render(const gll::Program& shaders);
   void setPosition(NormPos center) { m_center = center; }

 private:
   Animation m_animation;
   const MapCoordSys* m_coordSys = nullptr;
   NormPos m_center;
};


inline Effect::Effect(Animation animation, const MapCoordSys* cs, NormPos center)
: m_animation{std::move(animation)}, m_coordSys{cs}, m_center{center}
{
}

inline void Effect::render(const gll::Program& shaders)
{
   m_animation.render(shaders,
                      m_coordSys->toRenderCoords(m_center) - m_animation.size() / 2.f);
}
