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


std::string Program::linkLog() const
{
   if (!hasId())
      return "";

   GLint logLen = 0;
   glGetProgramiv(id(), GL_INFO_LOG_LENGTH, &logLen);
   std::string log(logLen, 0);
   glGetProgramInfoLog(id(), logLen, nullptr, log.data());
   return log;
}


void Program::use()
{
   if (hasId())
      glUseProgram(id());
}


Uniform Program::uniform(const GLchar *name) const
{
   if (hasId())
      return {};

   const GLint loc = glGetUniformLocation(id(), name);
   if (loc == -1)
      return {};

   return Uniform{id(), loc};
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
