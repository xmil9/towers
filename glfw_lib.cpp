//
// Oct-2020, Michael Lindner
// MIT license
//
#include "glfw_lib.h"
#include "glfw/glfw3.h"
#include <utility>


namespace glfwutil
{
///////////////////

GlfwLib::GlfwLib(ContextVersion ver, int profile)
: m_ver{std::move(ver)}, m_profile{profile}
{
}


GlfwLib::~GlfwLib()
{
   if (m_terminate)
      terminate();
}


GlfwErr GlfwLib::init()
{
   if (glfwInit())
   {
      setHints();
      m_terminate = true;
   }
   return glfwGetError(nullptr);
}


void GlfwLib::terminate()
{
   glfwTerminate();
}


void GlfwLib::setHints()
{
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_ver.major);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_ver.minor);
   glfwWindowHint(GLFW_OPENGL_PROFILE, m_profile);
#if defined(__APPLE__) && defined(__MACH__)
   if (m_ver.major >= 3)
      glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}

} // namespace glfwutil
