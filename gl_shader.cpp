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

bool Shader::create(GLenum shaderType)
{
   assert(!hasId());
   if (!hasId())
   {
      GlId id = glCreateShader(shaderType);
      setId(id);
   }
   return hasId();
}


bool Shader::compile()
{
   if (hasId())
   {
      glCompileShader(id());

      GLint success = GL_FALSE;
      glGetShaderiv(id(), GL_COMPILE_STATUS, &success);
      return success == GL_TRUE;
   }
   return false;
}


void Shader::setSource(GLsizei count, const GLchar** code, const GLint* length)
{
   if (hasId())
      glShaderSource(id(), count, code, length);
}


void Shader::destroy_(GlId id)
{
   glDeleteShader(id);
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
