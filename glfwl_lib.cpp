//
// Oct-2020, Michael Lindner
// MIT license
//
#include "glfwl_lib.h"
#include "glfw/glfw3.h"
#include <utility>


namespace glfwl
{
///////////////////

Lib::Lib(ContextVersion ver, int profile) : m_ver{std::move(ver)}, m_profile{profile}
{
}


Lib::~Lib()
{
   terminate();
}


Lib::Lib(Lib&& other)
{
   swap(*this, other);
}


Lib& Lib::operator=(Lib&& other)
{
   terminate();
   m_ver = other.m_ver;
   m_profile = other.m_profile;
   m_terminate = other.m_terminate;
   other.m_terminate = false;
   return *this;
}


Err Lib::init()
{
   if (glfwInit())
   {
      setHints();
      m_terminate = true;
   }
   return glfwGetError(nullptr);
}


void Lib::terminate()
{
   if (m_terminate)
   {
      glfwTerminate();
      m_terminate = false;
   }
}


void Lib::setHints()
{
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_ver.major);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_ver.minor);
   glfwWindowHint(GLFW_OPENGL_PROFILE, m_profile);
#if defined(__APPLE__) && defined(__MACH__)
   if (m_ver.major >= 3)
      glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}

} // namespace glfwl
