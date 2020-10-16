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

Program::~Program()
{
   destroy();
}


Program::Program(Program&& other)
{
   swap(*this, other);
}


Program& Program::operator=(Program&& other)
{
   destroy();
   swap(*this, other);
   return *this;
}


bool Program::create()
{
   assert(m_id == 0);
   if (m_id == 0)
      m_id = glCreateProgram();
   return m_id != 0;
}


void Program::destroy()
{
   if (m_id != 0)
   {
      glDeleteProgram(m_id);
      m_id = 0;
   }
}


void Program::attachShader(const Shader& shader)
{
   if (m_id != 0)
      glAttachShader(m_id, shader.id());
}


void Program::detachShader(const Shader& shader)
{
   if (m_id != 0)
      glDetachShader(m_id, shader.id());
}


bool Program::link()
{
   if (m_id != 0)
   {
      glLinkProgram(m_id);

      GLint success = GL_FALSE;
      glGetProgramiv(m_id, GL_LINK_STATUS, &success);
      return success == GL_TRUE;
   }
   return false;
}


void Program::use()
{
   if (m_id != 0)
      glUseProgram(m_id);
}

} // namespace glutil
