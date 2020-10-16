//
// Oct-2020, Michael Lindner
// MIT license
//
#pragma once
#include "glad/glad.h" // glad must be included before anything else opengl related.
#include "glfw/glfw3.h"
#include "glfw_types.h"
#include <utility>


namespace glfwutil
{
///////////////////

class GlfwLib
{
public:
   struct ContextVersion
   {
      unsigned int major = 1;
      unsigned int minor = 0;
   };

public:
   GlfwLib(ContextVersion ver, int profile);
   ~GlfwLib();
   GlfwLib(const GlfwLib&) = delete;
   GlfwLib(GlfwLib&& other);

   GlfwLib& operator=(const GlfwLib&) = delete;
   GlfwLib& operator=(GlfwLib&& other);

   GlfwErr init();
   void terminate();

   friend inline void swap(GlfwLib& a, GlfwLib& b)
   {
      std::swap(a.m_ver, b.m_ver);
      std::swap(a.m_profile, b.m_profile);
      std::swap(a.m_terminate, b.m_terminate);
   }

private:
   void setHints();

private:
   ContextVersion m_ver;
   int m_profile = GLFW_OPENGL_ANY_PROFILE;
   bool m_terminate = false;
};

} // namespace glfwutil
