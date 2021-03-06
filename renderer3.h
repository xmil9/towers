//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "spiel/camera_fps.h"
#include "spiel/direction.h"
#include "spiel/frustum3.h"
#include "glm/matrix.hpp"
#include "opengl_util/gll_data_format.h"
#include "opengl_util/gll_program.h"
#include "opengl_util/gll_shader.h"
#include "opengl_util/gll_texture.h"
#include "opengl_util/gll_uniform.h"
#include "opengl_util/gll_vao.h"
#include "opengl_util/gll_vbo.h"


class Renderer3
{
 public:
   bool setup();
   void cleanup();
   void renderFrame();

   void setFrustumAspect(float aspect) { m_frustum.setAspect(aspect); }
   void updateFrustumFov(float offset) { m_frustum.updateFov(offset); }
   void updateCameraDirection(const glm::vec2& offset) { m_cam.updateDirection(offset); }
   void updateCameraPosition(sp::DirectionXZ dir, float dist);

 private:
   bool setupShaders();
   bool setupTextures();
   bool setupData();
   bool setupRendering();
   bool setupLighting();

 private:
   sp::CameraFps m_cam;
   sp::Frustum3 m_frustum;

   // Matrix to transform object coords to world coords.
   glm::mat4 m_modelMat{0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
                        0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f};
   // Matrix to transform normals to world coords. Model matrix with scaling and
   // translations removed.
   glm::mat3 m_normalMat{0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f};

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


inline void Renderer3::updateCameraPosition(sp::DirectionXZ dir, float dist)
{
   m_cam.updatePosition(dir, dist);
}