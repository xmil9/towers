#include "glfw_lib.h"
#include "glfw_window.h"
#include <cassert>
#include <cstdlib>


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
static unsigned int vao;
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
static int shaderProg;


static void setupData()
{
   glGenVertexArrays(1, &vao);
   glGenBuffers(1, &vbo);
   glGenBuffers(1, &ebo);

   glBindVertexArray(vao);

   glBindBuffer(GL_ARRAY_BUFFER, vbo);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

   // The first argument has to match the 'location' value in the vertex shader code.
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);

   // Unbind buffers.
   glBindVertexArray(0);
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


static int setupShaders()
{
   int success;
   char infoLog[512];

   unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
   glShaderSource(vs, 1, &vertexShaderCode, nullptr);
   glCompileShader(vs);
   glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
   if (!success)
   {
      glGetShaderInfoLog(vs, 512, nullptr, infoLog);
      assert(false && "Vertex shader failed to compile.");
      assert(false && infoLog);
      return -1;
   }

   unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
   glShaderSource(fs, 1, &fragmentShaderCode, nullptr);
   glCompileShader(fs);
   glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
   if (!success)
   {
      glGetShaderInfoLog(fs, 512, nullptr, infoLog);
      assert(false && "Fragment shader failed to compile.");
      assert(false && infoLog);
      return -1;
   }

   unsigned int progr = glCreateProgram();
   glAttachShader(progr, vs);
   glAttachShader(progr, fs);
   glLinkProgram(progr);
   glGetProgramiv(progr, GL_LINK_STATUS, &success);
   if (!success)
   {
      glGetProgramInfoLog(progr, 512, NULL, infoLog);
      assert(false && "Shader program failed to link.");
      assert(false && infoLog);
      return -1;
   }

   glDeleteShader(vs);
   glDeleteShader(fs);

   return progr;
}


static void processInput(GLFWwindow* /*wnd*/)
{
   glfwPollEvents();
}


static void updateState()
{
}


static void render(GLFWwindow* wnd)
{
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   
   glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT);

   glUseProgram(shaderProg);
   glBindVertexArray(vao);
   glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

   glfwSwapBuffers(wnd);
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

   shaderProg = setupShaders();
   if (shaderProg == -1)
      return EXIT_FAILURE;

   setupData();

   while (!wnd.shouldClose())
   {
      processInput(wnd.handle());
      updateState();
      render(wnd.handle());
   }

   glDeleteVertexArrays(1, &vao);
   glDeleteBuffers(1, &vbo);
   glDeleteProgram(shaderProg);

   return EXIT_SUCCESS;
}
