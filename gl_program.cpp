//
// Oct-2020, Michael Lindner
// MIT license
//
#include "gl_program.h"
#include "gl_shader.h"
#include <cassert>


namespace glutil
{
///////////////////

void Program::attachShader(const Shader& shader)
{
   if (hasId())
      glAttachShader(id(), shader.id());
}


void Program::detachShader(const Shader& shader)
{
   if (hasId())
      glDetachShader(id(), shader.id());
}


bool Program::link()
{
   if (hasId())
   {
      glLinkProgram(id());

      GLint success = GL_FALSE;
      glGetProgramiv(id(), GL_LINK_STATUS, &success);
      return success == GL_TRUE;
   }
   return false;
}


void Program::use()
{
   if (hasId())
      glUseProgram(id());
}


GlId Program::create_()
{
   return glCreateProgram();
}


void Program::destroy_(GlId id)
{
   glDeleteProgram(id);
}

} // namespace glutil
