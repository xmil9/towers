//
// Dec-2020, Michael Lindner
// MIT license
//
#pragma once
#include "map_coord_sys.h"
#include "path.h"
#include "sprite.h"
#include "glm/glm.hpp"
#include <cstddef>

namespace gll
{
class Program;
}


///////////////////

class Defender
{
 public:
   Defender(Sprite sp, NormVec size, NormPos center, const MapCoordSys& cs);

   void render(const gll::Program& shaders);
   void update();

 private:
   void setPosition(NormPos center);
   void setSize(NormVec size);

 private:
   NormVec m_size;
   NormPos m_center;
   const MapCoordSys& m_coordSys;
   Sprite m_sprite;
};
