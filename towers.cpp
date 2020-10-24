#include "glfwl_lib.h"
#include "glfwl_window.h"
#include "gll_buffer.h"
#include "gll_data_format.h"
#include "gll_program.h"
#include "gll_shader.h"
#include "gll_uniform.h"
#include "gll_vertex_array.h"
#include "essentutils/filesys.h"
#include <cassert>
#include <cstdlib>
#include <utility>

constexpr glfwl::GlfwLib::ContextVersion OpenGLVersion{4, 6};

static float positions[] = {
   // Top
   0.0f, 0.6f, 0.0f,
   // Left
   -0.6f, 0.0f, 0.0f,
   // Right
   0.6f, 0.0f, 0.0f,
   // Bottom
   0.0f, -0.6f, 0.0f};
static constexpr gll::DataFormat posFormat = {3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                                              nullptr};

static unsigned int indices[] = {
   // Triangle 1
   0, 2, 1,
   // Triangle 2
   1, 2, 3};

static float colors[] = {
   // Top
   0.0f, 0.6f, 0.0f, 1.0f,
   // Left
   -0.6f, 0.0f, 0.0f, 1.0f,
   // Right
   0.6f, 0.0f, 0.0f, 1.0f,
   // Bottom
   0.0f, -0.6f, 0.0f, 1.0f};
static constexpr gll::DataFormat colorFormat = {4, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                                                nullptr};

static gll::VertexArray vao;
static gll::Buffer posBuf;
static gll::Buffer colorBuf;
static gll::Buffer elemBuf;
static gll::Program prog;


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

   elemBuf.create();
   elemBuf.bind(GL_ELEMENT_ARRAY_BUFFER);
   elemBuf.setData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

   // Unbind vao first to stop "recording" information in it.
   vao.unbind();
   // Unbind each buffer type from global state.
   gll::Buffer::unbind(GL_ARRAY_BUFFER);
   gll::Buffer::unbind(GL_ELEMENT_ARRAY_BUFFER);
}


static void setupRendering()
{
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
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


static void processInput(glfwl::Window& /*wnd*/)
{
   glfwPollEvents();
}


static void updateState()
{
}


static void render(glfwl::Window& wnd)
{
   glClear(GL_COLOR_BUFFER_BIT);

   prog.use();

   // const float time = static_cast<float>(glfwGetTime());
   // const float green = sin(time) / 2.0f + 0.5f;
   // gll::Uniform color = prog.uniform("color");
   // color.setValue(glm::vec4(0.0f, green, 0.0f, 1.0f));

   vao.bind();
   glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(std::size(indices)), GL_UNSIGNED_INT,
                  nullptr);
   vao.unbind();

   wnd.swapBuffers();
}


int main()
{
   glfwl::GlfwLib glfw{OpenGLVersion, GLFW_OPENGL_CORE_PROFILE};
   glfwl::GlfwErr err = glfw.init();
   if (err)
      return EXIT_FAILURE;

   glfwl::Window wnd;
   err = wnd.create(800, 600, "towers");
   if (err)
      return EXIT_FAILURE;

   glfwMakeContextCurrent(wnd.handle());

   if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
      return EXIT_FAILURE;

   prog = std::move(setupShaders());
   if (!prog)
      return EXIT_FAILURE;
   setupData();
   setupRendering();

   while (!wnd.shouldClose())
   {
      processInput(wnd);
      updateState();
      render(wnd);
   }

   vao.destroy();
   posBuf.destroy();
   colorBuf.destroy();
   elemBuf.destroy();

   return EXIT_SUCCESS;
}
