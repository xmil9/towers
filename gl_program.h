//
// Oct-2020, Michael Lindner
// MIT license
//
#pragma once
#include "glad/glad.h" // glad must be included before anything else opengl related.
#include "gl_types.h"
#include <utility>

namespace glutil { class Shader; }


namespace glutil
{
///////////////////

class Program
{
 public:
   Program() = default;
   ~Program();
   Program(const Program&) = delete;
   Program(Program&& other);

   Program& operator=(const Program&) = delete;
   Program& operator=(Program&& other);
   explicit operator bool() const { return m_id != 0; }
   bool operator!() const { return !operator bool(); }

   GlId id() const { return m_id; }
   bool create();
   void destroy();
   void attachShader(const Shader& shader);
   void detachShader(const Shader& shader);
   bool link();
   void use();

   friend inline void swap(Program& a, Program& b)
   {
      std::swap(a.m_id, b.m_id);
   }

 private:
   GlId m_id = 0;
};

} // namespace glutil
