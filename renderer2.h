//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "camera_2d.h"
#include "frustum2.h"
#include "sprite.h"
#include "gll_program.h"
#include <vector>

class Resources;


class Renderer2
{
public:
   explicit Renderer2(Resources* resources);

   bool setupShaders();
   void render(const std::vector<Sprite>& sprites) const;

private:
   Resources* m_resources = nullptr;
   Camera2d m_cam;
   Frustum2 m_frustum;
   gll::Program m_shaders;
};


inline Renderer2::Renderer2(Resources* resources)
   : m_resources{resources}
{
}
