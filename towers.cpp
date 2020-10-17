#include "gl_program.h"
#include "gl_shader.h"
#include "gl_vertex_array.h"
#include "glfw_lib.h"
#include "glfw_window.h"
#include <cassert>
#include <cstdlib>
#include <utility>


static float vertices[] = {
   // Top
   0.0f, 0.6f, 0.0f,
   // Left
   -0.6f, 0.0f, 0.0f,
   // Right
   0.6f, 0.0f, 0.0f,
   // Bottom
   0.0f, -0.6f, 0.0f};
static unsigned int indices[] = {
   // Triangle 1
   0, 2, 1,
   // Triangle 2
   1, 2, 3};
static glutil::VertexArray vao;
static unsigned int vbo;
static unsigned int ebo;
static const char* vertexShaderCode = "#version 330 core\n"
                                      "layout (location = 0) in vec3 pos;\n"
                                      "void main()\n"
                                      "{\n"
                                      "  gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);\n"
                                      "}\0";
static const char* fragmentShaderCode = "#version 330 core\n"
                                        "out vec4 fragColor;\n"
                                        "void main()\n"
                                        "{\n"
                                        "  fragColor = vec4(0.2f, 0.5f, 1.0f, 1.0f);\n"
                                        "}\0";
static glutil::Program prog;


static void setupData()
{
   vao.create();
   glGenBuffers(1, &vbo);
   glGenBuffers(1, &ebo);

   vao.bind();

   glBindBuffer(GL_ARRAY_BUFFER, vbo);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

   // The first argument has to match the 'location' value in the vertex shader code.
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);

   // Unbind buffers.
   vao.unbind();
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


static glutil::Program setupShaders()
{
   bool ok = true;

   glutil::Shader vs{glutil::makeVertexShader(vertexShaderCode)};
   ok = vs.compile();

   glutil::Shader fs{glutil::makeFragmentShader(fragmentShaderCode)};
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
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   
   glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT);

   prog.use();
   vao.bind();
   glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

   wnd.swapBuffers();
}


int main()
{
   glfwutil::GlfwLib glfw{{3, 3}, GLFW_OPENGL_CORE_PROFILE};
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

   while (!wnd.shouldClose())
   {
      processInput(wnd);
      updateState();
      render(wnd);
   }

   vao.destroy();
   glDeleteBuffers(1, &vbo);

   return EXIT_SUCCESS;
}
