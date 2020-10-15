//
// Oct-2020, Michael Lindner
// MIT license
//
#pragma once
#include "glad/glad.h" // glad must be included before anything else opengl related.


namespace glutil
{
///////////////////

class Shader
{
 public:
   explicit Shader(GLenum shaderType);
   ~Shader();
   Shader(const Shader&) = delete;
   Shader(Shader&&) = default;

   Shader& operator=(const Shader&) = delete;
   Shader& operator=(Shader&&) = default;
   explicit operator bool() const { return m_id != 0; }
   bool operator!() const { return !operator bool(); }

   bool create();
   void destroy();
   bool compile();
   GLuint id() const { return m_id; }

   void setSource(const GLchar* code) { setSource(1, &code, nullptr); }
   void setSource(GLsizei count, const GLchar** code, const GLint* length);

 private:
   GLenum m_type = GL_NONE;
   GLuint m_id = 0;
};

} // namespace glutil
