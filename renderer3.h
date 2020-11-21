//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "app_types.h"
#include "camera_fps.h"
#include "frustum3.h"
#include "gll_data_format.h"
#include "gll_program.h"
#include "gll_shader.h"
#include "gll_texture.h"
#include "gll_uniform.h"
#include "gll_vao.h"
#include "gll_vbo.h"
#include "glm/matrix.hpp"


class Renderer3
{
 public:
   bool setup();
   void cleanup();
   void renderFrame();

   void setFrustumAspect(float aspect) { m_frustum.setAspect(aspect); }
   void updateFrustumFov(float offset) { m_frustum.updateFov(offset); }
   void updateCameraDirection(const glm::vec2& offset) { m_cam.updateDirection(offset); }
   void updateCameraPosition(DirectionXZ dir, float dist);

 private:
   bool setupShaders();
   bool setupTextures();
   bool setupData();
   bool setupRendering();
   bool setupLighting();

 private:
   CameraFps m_cam;
   Frustum3 m_frustum;

   // Matrix to transform object coords to world coords.
   glm::mat4 m_modelMat;
   // Matrix to transform normals to world coords. Model matrix with scaling and
   // translations removed.
   glm::mat3 m_normalMat;

   gll::Vao m_vao;
   gll::Vbo m_posBuf;
   gll::Vbo m_normalBuf;
   gll::Vbo m_colorBuf;
   gll::Vbo m_texCoordBuf;
   gll::Texture2D m_tex;
   gll::Texture2D m_tex2;
   gll::Vbo m_elemBuf;
   gll::Program m_prog;
};


inline void Renderer3::updateCameraPosition(DirectionXZ dir, float dist)
{
   m_cam.updatePosition(dir, dist);
}