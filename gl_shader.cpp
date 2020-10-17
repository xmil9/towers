//
// Oct-2020, Michael Lindner
// MIT license
//
#include "gl_shader.h"
#include <cassert>


namespace
{
///////////////////

glutil::Shader makeShader(GLenum shaderType, const GLchar* code)
{
   glutil::Shader s;
   if (s.create(shaderType))
      s.setSource(code);
   return s;
}

} // namespace


namespace glutil
{
///////////////////

Shader::Shader(GlId id)
   : m_id{id}
{
}


Shader::~Shader()
{
   destroy();
}


Shader::Shader(Shader&& other)
{
   swap(*this, other);
}


Shader& Shader::operator=(Shader&& other)
{
   destroy();
   swap(*this, other);
   return *this;
}


bool Shader::create(GLenum shaderType)
{
   assert(m_id == 0);
   if (m_id == 0)
      m_id = glCreateShader(shaderType);
   return m_id != 0;
}


void Shader::destroy()
{
   if (m_id != 0)
   {
      glDeleteShader(m_id);
      m_id = 0;
   }
}


void Shader::attach(GlId id)
{
   destroy();
   m_id = id;
}


GlId Shader::detach()
{
   GlId id = m_id;
   m_id = 0;
   return id;
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


///////////////////

Shader makeVertexShader(const GLchar* code)
{
   return makeShader(GL_VERTEX_SHADER, code);
}


Shader makeFragmentShader(const GLchar* code)
{
   return makeShader(GL_FRAGMENT_SHADER, code);
}

} // namespace glutil
