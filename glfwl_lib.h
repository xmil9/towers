//
// Oct-2020, Michael Lindner
// MIT license
//
#pragma once
#include "glad/glad.h" // glad must be included before anything else opengl related.
#include "glfw/glfw3.h"
#include "glfwl_types.h"
#include <utility>


namespace glfwl
{
///////////////////

class Lib
{
 public:
   struct ContextVersion
   {
      unsigned int major = 1;
      unsigned int minor = 0;
   };

 public:
   Lib(ContextVersion ver, int profile);
   ~Lib();
   Lib(const Lib&) = delete;
   Lib(Lib&& other);

   Lib& operator=(const Lib&) = delete;
   Lib& operator=(Lib&& other);

   Err init();
   void terminate();

   friend inline void swap(Lib& a, Lib& b)
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

} // namespace glfwl
