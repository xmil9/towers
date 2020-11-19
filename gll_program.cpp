//
// Oct-2020, Michael Lindner
// MIT license
//
#include "gll_program.h"
#include "gll_shader.h"
#include <cassert>


namespace gll
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


void Program::use() const
{
   if (hasId())
      glUseProgram(id());
}


Uniform Program::uniform(const GLchar* name) const
{
   if (!hasId())
      return {};

   const GLint loc = glGetUniformLocation(id(), name);
   if (loc == -1)
      return {};

   return Uniform{id(), loc};
}


void Program::setTextureUnit(const GLchar* samplerName, GLint unitIdx) const
{
   if (!hasId())
      return;
   
   // Program need to be in-use!
   gll::Uniform sampler = uniform(samplerName);
   sampler.setValue(unitIdx);
}


ObjId Program::create_()
{
   return glCreateProgram();
}


void Program::destroy_(ObjId id)
{
   glDeleteProgram(id);
}

} // namespace gll
