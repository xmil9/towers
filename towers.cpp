#include "gl_buffer.h"
#include "gl_data_format.h"
#include "gl_program.h"
#include "gl_shader.h"
#include "gl_uniform.h"
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
static constexpr glutil::DataFormat posFormat = {3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
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
static constexpr glutil::DataFormat colorFormat = {4, GL_FLOAT, GL_FALSE,
                                                   4 * sizeof(float), nullptr};

static glutil::VertexArray vao;
static glutil::Buffer posBuf;
static glutil::Buffer colorBuf;
static glutil::Buffer elemBuf;
static glutil::Program prog;


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
   glutil::Buffer::unbind(GL_ARRAY_BUFFER);
   glutil::Buffer::unbind(GL_ELEMENT_ARRAY_BUFFER);
}


static void setupRendering()
{
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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

   // const float time = static_cast<float>(glfwGetTime());
   // const float green = sin(time) / 2.0f + 0.5f;
   // glutil::Uniform color = prog.uniform("color");
   // color.setValue(glm::vec4(0.0f, green, 0.0f, 1.0f));

   vao.bind();
   glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(std::size(indices)), GL_UNSIGNED_INT,
                  nullptr);
   vao.unbind();

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
   posBuf.destroy();
   colorBuf.destroy();
   elemBuf.destroy();

   return EXIT_SUCCESS;
}
