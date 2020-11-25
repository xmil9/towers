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
   bool setup(Resources* resources, int viewWidth, int viewHeight);
   void setFrustumSize(int width, int height) { m_frustum.setSize(width, height); }

   void beginRendering() const;
   const gll::Program& shaders() const { return m_shaders; }

private:
   bool setupShaders();
   bool setupSetting(int viewWidth, int viewHeight);

private:
   Resources* m_resources = nullptr;
   Camera2d m_cam;
   Frustum2 m_frustum;
   gll::Program m_shaders;
};
