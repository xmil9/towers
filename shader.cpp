//
// Oct-2020, Michael Lindner
// MIT license
//
#include "shader.h"
#include <cassert>


namespace glutil
{
///////////////////

Shader::Shader(GLenum shaderType) : m_type{shaderType}
{
}


Shader::~Shader()
{
   destroy();
}


bool Shader::create()
{
   assert(m_id == 0);
   if (m_id == 0)
      m_id = glCreateShader(m_type);
   return m_id != 0;
}


void Shader::destroy()
{
   if (m_id != 0)
      glDeleteShader(m_id);
}


bool Shader::compile()
{
   if (m_id != 0)
   {
      glCompileShader(m_id);

      GLint success = GL_FALSE;
      glGetShaderiv(m_id, GL_COMPILE_STATUS, &success);
      return success == GL_TRUE;
   }
   return false;
}


void Shader::setSource(GLsizei count, const GLchar** code, const GLint* length)
{
   if (m_id != 0)
      glShaderSource(m_id, count, code, length);
}

} // namespace glutil
