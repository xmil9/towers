//
// Oct-2020, Michael Lindner
// MIT license
//
#include "app_window.h"
#include "camera_fps.h"
#include "frustum.h"
#include "input_state.h"
#include "lap_clock.h"
#include "glfwl_lib.h"
#include "gll_buffer.h"
#include "gll_data_format.h"
#include "gll_program.h"
#include "gll_shader.h"
#include "gll_texture.h"
#include "gll_uniform.h"
#include "gll_vertex_array.h"
#include "essentutils/filesys.h"
#include <cassert>
#include <cstdlib>
#include <utility>

constexpr glfwl::GlfwLib::ContextVersion OpenGLVersion{4, 6};

static float positions[] = {
   // Front face
   // lt
   -0.6f,
   0.6f,
   0.0f,
   // rt
   0.6f,
   0.6f,
   0.0f,
   // rb
   0.6f,
   -0.6f,
   0.0f,
   // lb
   -0.6f,
   -0.6f,
   0.0f,
   // Left face
   // lt
   -0.6f,
   0.6f,
   -1.2f,
   // rt
   -0.6f,
   0.6f,
   0.0f,
   // rb
   -0.6f,
   -0.6f,
   0.0f,
   // lb
   -0.6f,
   -0.6f,
   -1.2f,
   // Top face
   // lt
   -0.6f,
   0.6f,
   -1.2f,
   // rt
   0.6f,
   0.6f,
   -1.2f,
   // rb
   0.6f,
   0.6f,
   0.0f,
   // lb
   -0.6f,
   0.6f,
   0.0f,
   // Right face
   // lt
   0.6f,
   0.6f,
   0.0f,
   // rt
   0.6f,
   0.6f,
   -1.2f,
   // rb
   0.6f,
   -0.6f,
   -1.2f,
   // lb
   0.6f,
   -0.6f,
   0.0f,
   // Bottom face
   // lt
   0.6f,
   -0.6f,
   -1.2f,
   // rt
   -0.6f,
   -0.6f,
   -1.2f,
   // rb
   -0.6f,
   -0.6f,
   0.0f,
   // lb
   0.6f,
   -0.6f,
   0.0f,
   // Back face
   // lt
   0.6f,
   0.6f,
   -1.2f,
   // rt
   -0.6f,
   0.6f,
   -1.2f,
   // rb
   -0.6f,
   -0.6f,
   -1.2f,
   // lb
   0.6f,
   -0.6f,
   -1.2f,
};
static constexpr gll::DataFormat posFormat = {3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                                              nullptr};

static unsigned int indices[] = {
   // Front face
   // Triangle 1
   0,
   1,
   2,
   // Triangle 2
   2,
   3,
   0,
   // Left face
   // Triangle 1
   4,
   5,
   6,
   // Triangle 2
   6,
   7,
   4,
   // Top face
   // Triangle 1
   8,
   9,
   10,
   // Triangle 2
   10,
   11,
   8,
   // Right face
   // Triangle 1
   12,
   13,
   14,
   // Triangle 2
   14,
   15,
   12,
   // Bottom face
   // Triangle 1
   16,
   17,
   18,
   // Triangle 2
   18,
   19,
   16,
   // Back face
   // Triangle 1
   20,
   21,
   22,
   // Triangle 2
   22,
   23,
   20,
};

static float colors[] = {
   // Front face
   // lt
   0.0f,
   0.6f,
   0.0f,
   1.0f,
   // rt
   -0.6f,
   0.0f,
   0.0f,
   1.0f,
   // rb
   0.6f,
   0.0f,
   0.0f,
   1.0f,
   // lb
   0.0f,
   -0.6f,
   0.0f,
   1.0f,
   // Left face
   // lt
   0.0f,
   0.6f,
   0.0f,
   1.0f,
   // rt
   -0.6f,
   0.0f,
   0.0f,
   1.0f,
   // rb
   0.6f,
   0.0f,
   0.0f,
   1.0f,
   // lb
   0.0f,
   -0.6f,
   0.0f,
   1.0f,
   // Top face
   // lt
   0.0f,
   0.6f,
   0.0f,
   1.0f,
   // rt
   -0.6f,
   0.0f,
   0.0f,
   1.0f,
   // rb
   0.6f,
   0.0f,
   0.0f,
   1.0f,
   // lb
   0.0f,
   -0.6f,
   0.0f,
   1.0f,
   // Right face
   // lt
   0.0f,
   0.6f,
   0.0f,
   1.0f,
   // rt
   -0.6f,
   0.0f,
   0.0f,
   1.0f,
   // rb
   0.6f,
   0.0f,
   0.0f,
   1.0f,
   // lb
   0.0f,
   -0.6f,
   0.0f,
   1.0f,
   // Bottom face
   // lt
   0.0f,
   0.6f,
   0.0f,
   1.0f,
   // rt
   -0.6f,
   0.0f,
   0.0f,
   1.0f,
   // rb
   0.6f,
   0.0f,
   0.0f,
   1.0f,
   // lb
   0.0f,
   -0.6f,
   0.0f,
   1.0f,
   // Back face
   // lt
   0.0f,
   0.6f,
   0.0f,
   1.0f,
   // rt
   -0.6f,
   0.0f,
   0.0f,
   1.0f,
   // rb
   0.6f,
   0.0f,
   0.0f,
   1.0f,
   // lb
   0.0f,
   -0.6f,
   0.0f,
   1.0f,
};
static constexpr gll::DataFormat colorFormat = {4, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                                                nullptr};

// Texture coord system:
// (0, 0) left-bottom
// (1, 1) right-top
static float texCoords[] = {
   // Front face
   // lt
   0.0f,
   1.0f,
   // rt
   1.0f,
   1.0f,
   // rb
   1.0f,
   0.0f,
   // lb
   0.0f,
   0.0f,
   // Left face
   // lt
   0.0f,
   1.0f,
   // rt
   1.0f,
   1.0f,
   // rb
   1.0f,
   0.0f,
   // lb
   0.0f,
   0.0f,
   // Top face
   // lt
   0.0f,
   1.0f,
   // rt
   1.0f,
   1.0f,
   // rb
   1.0f,
   0.0f,
   // lb
   0.0f,
   0.0f,
   // Right face
   // lt
   0.0f,
   1.0f,
   // rt
   1.0f,
   1.0f,
   // rb
   1.0f,
   0.0f,
   // lb
   0.0f,
   0.0f,
   // Bottom face
   // lt
   0.0f,
   1.0f,
   // rt
   1.0f,
   1.0f,
   // rb
   1.0f,
   0.0f,
   // lb
   0.0f,
   0.0f,
   // Back face
   // lt
   0.0f,
   1.0f,
   // rt
   1.0f,
   1.0f,
   // rb
   1.0f,
   0.0f,
   // lb
   0.0f,
   0.0f,
};
static constexpr gll::DataFormat texCoordFormat = {2, GL_FLOAT, GL_FALSE,
                                                   2 * sizeof(float), nullptr};

static gll::VertexArray vao;
static gll::Buffer posBuf;
static gll::Buffer colorBuf;
static gll::Buffer texCoordBuf;
static gll::Texture2D tex;
static gll::Texture2D tex2;
static gll::Buffer elemBuf;
static gll::Program prog;

// Sequence of coord tranformations:
// obj space -> world space -> view/camera space -> clip space -> screen space
//
// Matrix to transform object coords to world coords.
static glm::mat4 model;
// Matrix to transform world coords to camera view coords.
static glm::mat4 view;
// Matrix to transform view coords to clip space (frustum) coords.
// Note: A final viewport transformation will happen from clip space to 2D screen space.
//       The matrix of this transformation is defined by glViewport.
static glm::mat4 projection;



static bool setupTextures()
{
   const std::filesystem::path appPath = sutil::appDirectory();
   if (appPath.empty())
      return false;

   tex.create();
   tex.bind();
   tex.setWrapPolicy(GL_REPEAT, GL_REPEAT);
   tex.setScaleFiltering(GL_NEAREST, GL_NEAREST);
   tex.loadData(appPath / "directions.png", true, 0, GL_RGB, GL_RGBA, GL_UNSIGNED_BYTE);
   tex.generateMipmap();

   tex2.create();
   tex2.bind();
   tex2.setWrapPolicy(GL_REPEAT, GL_REPEAT);
   tex2.setScaleFiltering(GL_NEAREST, GL_NEAREST);
   tex2.loadData(appPath / "red_marble.png", true, 0, GL_RGB, GL_RGBA, GL_UNSIGNED_BYTE);
   tex2.generateMipmap();

   prog.use();
   prog.setTextureUnit("texSampler", 0);
   prog.setTextureUnit("texSampler2", 1);

   return true;
}


static void setupData()
{
   vao.create();
   vao.bind();

   posBuf.create();
   posBuf.bind(GL_ARRAY_BUFFER);
   posBuf.setData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
   // The attribute index has to match the 'location' value in the vertex shader code.
   constexpr GLuint posAttribIdx = 0;
   vao.setAttribFormat(posAttribIdx, posFormat);
   vao.enableAttrib(posAttribIdx);

   colorBuf.create();
   colorBuf.bind(GL_ARRAY_BUFFER);
   colorBuf.setData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
   // The attribute index has to match the 'location' value in the vertex shader code.
   constexpr GLuint colorAttribIdx = 1;
   vao.setAttribFormat(colorAttribIdx, colorFormat);
   vao.enableAttrib(colorAttribIdx);

   texCoordBuf.create();
   texCoordBuf.bind(GL_ARRAY_BUFFER);
   texCoordBuf.setData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);
   // The attribute index has to match the 'location' value in the vertex shader code.
   constexpr GLuint texCoordAttribIdx = 2;
   vao.setAttribFormat(texCoordAttribIdx, texCoordFormat);
   vao.enableAttrib(texCoordAttribIdx);

   elemBuf.create();
   elemBuf.bind(GL_ELEMENT_ARRAY_BUFFER);
   elemBuf.setData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

   // Unbind vao first to stop "recording" information in it.
   vao.unbind();
   // Unbind each buffer type from global state.
   gll::Buffer::unbind(GL_ARRAY_BUFFER);
   gll::Buffer::unbind(GL_ELEMENT_ARRAY_BUFFER);
   gll::Texture2D::unbind();
}


static void setupRendering(FrameClock& frameClock)
{
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
   glEnable(GL_DEPTH_TEST);

   model = glm::mat4(1.0f);
   model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(1.0f, 0.0f, 0.0f));

   frameClock.nextLap();
}


static gll::Program setupShaders()
{
   const std::filesystem::path appPath = sutil::appDirectory();
   bool ok = !appPath.empty();

   gll::Shader vs{gll::makeVertexShader(appPath / "towers.vs")};
   if (ok)
      ok = vs.compile();

   gll::Shader fs{gll::makeFragmentShader(appPath / "towers.fs")};
   if (ok)
      ok = fs.compile();

   gll::Program shaderProg;
   if (ok)
      ok = shaderProg.create();
   if (ok)
   {
      shaderProg.attachShader(vs);
      shaderProg.attachShader(fs);
      ok = shaderProg.link();
   }

   if (!ok)
      shaderProg.destroy();
   return shaderProg;
}


static void updateState()
{
}


static void render(glfwl::Window& wnd, FrameClock& frameClock, const CameraFps& cam,
                   const Frustum& frustum)
{
   frameClock.nextLap();

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glActiveTexture(GL_TEXTURE0);
   tex.bind();
   glActiveTexture(GL_TEXTURE1);
   tex2.bind();

   prog.use();
   vao.bind();

   view = cam.viewMatrix();
   projection = frustum.projectionMatrix();

   gll::Uniform modelUf = prog.uniform("model");
   modelUf.setValue(model);
   gll::Uniform viewUf = prog.uniform("view");
   viewUf.setValue(view);
   gll::Uniform projUf = prog.uniform("projection");
   projUf.setValue(projection);

   glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(std::size(indices)), GL_UNSIGNED_INT,
                  nullptr);

   gll::VertexArray::unbind();
   gll::Texture2D::unbind();

   wnd.swapBuffers();
   wnd.pollEvents();
}


int main()
{
   glfwl::GlfwLib glfw{OpenGLVersion, GLFW_OPENGL_CORE_PROFILE};
   glfwl::GlfwErr err = glfw.init();
   if (err)
      return EXIT_FAILURE;

   FrameClock frameClock;
   InputState input;
   CameraFps cam;
   cam.setupInput(input);
   Frustum frustum;
   frustum.setupInput(input);

   AppWindow wnd;
   wnd.setInputController(&input);
   err = wnd.create(800, 800, "towers");
   if (err)
      return EXIT_FAILURE;
   wnd.setCursorMode(GLFW_CURSOR_DISABLED);

   glfwMakeContextCurrent(wnd.handle());

   if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
      return EXIT_FAILURE;

   prog = std::move(setupShaders());
   if (!prog)
      return EXIT_FAILURE;
   if (!setupTextures())
      return EXIT_FAILURE;
   setupData();
   setupRendering(frameClock);

   while (!wnd.shouldClose())
   {
      input.pollInput(wnd, frameClock.lapLength(MsToSecs));
      updateState();
      render(wnd, frameClock, cam, frustum);
   }

   vao.destroy();
   posBuf.destroy();
   colorBuf.destroy();
   texCoordBuf.destroy();
   tex.destroy();
   elemBuf.destroy();

   return EXIT_SUCCESS;
}
