#include "gl_buffer.h"
#include "gl_data_format.h"
#include "gl_program.h"
#include "gl_shader.h"
#include "gl_vertex_array.h"
#include "glfw_lib.h"
#include "glfw_window.h"
#include "essentutils/filesys.h"
#include <cassert>
#include <cstdlib>
#include <utility>

constexpr glfwutil::GlfwLib::ContextVersion OpenGLVersion{4, 6};

static float positions[] = {
   // Top
   0.0f, 0.6f, 0.0f,
   // Left
   -0.6f, 0.0f, 0.0f,
   // Right
   0.6f, 0.0f, 0.0f,
   // Bottom
   0.0f, -0.6f, 0.0f};
static glutil::DataFormat posFormat = {3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr};
static unsigned int indices[] = {
   // Triangle 1
   0, 2, 1,
   // Triangle 2
   1, 2, 3};
static glutil::VertexArray vao;
static glutil::Buffer vbo;
static glutil::Buffer ebo;
static glutil::Program prog;


static void setupData()
{
   vao.create();
   vbo.create();
   ebo.create();

   vao.bind();

   vbo.bind(GL_ARRAY_BUFFER);
   vbo.setData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
   ebo.bind(GL_ELEMENT_ARRAY_BUFFER);
   ebo.setData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

   // The attribute index has to match the 'location' value in the vertex shader code.
   constexpr GLuint posAttribIdx = 0;
   vao.setAttribFormat(posAttribIdx, posFormat);
   vao.enableAttrib(posAttribIdx);

   // Unbind buffers.
   vao.unbind();
   vbo.unbind(GL_ARRAY_BUFFER);
   ebo.unbind(GL_ELEMENT_ARRAY_BUFFER);
}


static void setupRendering()
{
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
}


static glutil::Program setupShaders()
{
   const std::filesystem::path appPath = sutil::appDirectory();
   bool ok = !appPath.empty();

   glutil::Shader vs{glutil::makeVertexShader(appPath / "towers.vs")};
   if (ok)
      ok = vs.compile();

   glutil::Shader fs{glutil::makeFragmentShader(appPath / "towers.fs")};
   if (ok)
      ok = fs.compile();

   glutil::Program shaderProg;
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


static void processInput(glfwutil::Window& /*wnd*/)
{
   glfwPollEvents();
}


static void updateState()
{
}


static void render(glfwutil::Window& wnd)
{
   glClear(GL_COLOR_BUFFER_BIT);

   prog.use();
   vao.bind();
   glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

   wnd.swapBuffers();
}


int main()
{
   glfwutil::GlfwLib glfw{OpenGLVersion, GLFW_OPENGL_CORE_PROFILE};
   glfwutil::GlfwErr err = glfw.init();
   if (err)
      return EXIT_FAILURE;

   glfwutil::Window wnd;
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
   vbo.destroy();
   ebo.destroy();

   return EXIT_SUCCESS;
}
