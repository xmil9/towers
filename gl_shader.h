//
// Oct-2020, Michael Lindner
// MIT license
//
#pragma once
#include "glad/glad.h" // glad must be included before anything else opengl related.
#include "gl_types.h"
#include <utility>


namespace glutil
{
///////////////////

class Shader
{
 public:
   Shader() = default;
   explicit Shader(GlId id);
   ~Shader();
   Shader(const Shader&) = delete;
   Shader(Shader&& other);

   Shader& operator=(const Shader&) = delete;
   Shader& operator=(Shader&& other);
   explicit operator bool() const { return m_id != 0; }
   bool operator!() const { return !operator bool(); }

   GlId id() const { return m_id; }
   bool create(GLenum shaderType);
   void destroy();
   void attach(GlId id);
   GlId detach();
   bool compile();

   void setSource(const GLchar* code) { setSource(1, &code, nullptr); }
   void setSource(GLsizei count, const GLchar** code, const GLint* length);

   friend inline void swap(Shader& a, Shader& b)
   {
      std::swap(a.m_id, b.m_id);
   }

 private:
   GlId m_id = 0;
};


///////////////////

Shader makeVertexShader(const GLchar* code);
Shader makeFragmentShader(const GLchar* code);

} // namespace glutil
