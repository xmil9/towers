//
// Oct-2020, Michael Lindner
// MIT license
//
#include "gll_shader.h"
#include <cassert>
#include <fstream>
#include <sstream>


namespace
{
///////////////////

gll::Shader makeShader(GLenum shaderType, const GLchar* code)
{
   gll::Shader s;
   if (s.create(shaderType))
      s.setSource(code);
   return s;
}


gll::Shader makeShader(GLenum shaderType, const std::filesystem::path& codeFile)
{
   gll::Shader s;
   if (s.create(shaderType))
      s.loadSource(codeFile);
   return s;
}

} // namespace


namespace gll
{
///////////////////

bool Shader::create(GLenum shaderType)
{
   assert(!hasId());
   if (!hasId())
   {
      ObjId id = glCreateShader(shaderType);
      setId(id);
   }
   return hasId();
}


void Shader::setSource(GLsizei count, const GLchar** code, const GLint* length)
{
   if (hasId())
      glShaderSource(id(), count, code, length);
}


bool Shader::loadSource(const std::filesystem::path& path)
{
   if (!hasId())
      return false;

   std::ifstream fileStream{path.c_str()};
   std::stringstream buffer;
   buffer << fileStream.rdbuf();

   setSource(buffer.str().c_str());
   return true;
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


std::string Shader::compileLog() const
{
   if (!hasId())
      return "";

   GLint logLen = 0;
   glGetShaderiv(id(), GL_INFO_LOG_LENGTH, &logLen);
   std::string log(logLen, 0);
   glGetShaderInfoLog(id(), logLen, nullptr, log.data());
   return log;
}


void Shader::destroy_(ObjId id)
{
   glDeleteShader(id);
}


///////////////////

Shader makeVertexShader(const GLchar* code)
{
   return makeShader(GL_VERTEX_SHADER, code);
}


Shader makeVertexShader(const std::filesystem::path& codeFile)
{
   return makeShader(GL_VERTEX_SHADER, codeFile);
}


Shader makeFragmentShader(const GLchar* code)
{
   return makeShader(GL_FRAGMENT_SHADER, code);
}


Shader makeFragmentShader(const std::filesystem::path& codeFile)
{
   return makeShader(GL_FRAGMENT_SHADER, codeFile);
}

} // namespace gll
