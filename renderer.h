//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "app_types.h"
#include "camera_fps.h"
#include "frustum.h"
#include "gll_buffer.h"
#include "gll_data_format.h"
#include "gll_program.h"
#include "gll_shader.h"
#include "gll_texture.h"
#include "gll_uniform.h"
#include "gll_vertex_array.h"
#include "glm/matrix.hpp"


class Renderer
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

 private:
   CameraFps m_cam;
   Frustum m_frustum;

   // Matrix to transform object coords to world coords.
   glm::mat4 m_model;

   gll::VertexArray m_vao;
   gll::Buffer m_posBuf;
   gll::Buffer m_colorBuf;
   gll::Buffer m_texCoordBuf;
   gll::Texture2D m_tex;
   gll::Texture2D m_tex2;
   gll::Buffer m_elemBuf;
   gll::Program m_prog;
};


inline void Renderer::updateCameraPosition(DirectionXZ dir, float dist)
{
   m_cam.updatePosition(dir, dist);
}