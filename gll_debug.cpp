//
// Nov-2020, Michael Lindner
// MIT license
//
#include "gll_debug.h"
#include <iostream>
#include <string>


namespace
{
///////////////////

std::string sourceToString(GLenum source)
{
   switch (source)
   {
   case GL_DEBUG_SOURCE_API:
      return "Api";
   case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
      return "Window system";
   case GL_DEBUG_SOURCE_SHADER_COMPILER:
      return "Shader compiler";
   case GL_DEBUG_SOURCE_THIRD_PARTY:
      return "Thrid party";
   case GL_DEBUG_SOURCE_APPLICATION:
      return "Application";
   case GL_DEBUG_SOURCE_OTHER:
      return "Other";
   default:
      return "";
   }
}


std::string typeToString(GLenum type)
{
   switch (type)
   {
   case GL_DEBUG_TYPE_ERROR:
      return "Error";
   case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
      return "Deprecated behavior";
   case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
      return "Undefined behavior";
   case GL_DEBUG_TYPE_PORTABILITY:
      return "Portability";
   case GL_DEBUG_TYPE_PERFORMANCE:
      return "Performance";
   case GL_DEBUG_TYPE_MARKER:
      return "Marker";
   case GL_DEBUG_TYPE_PUSH_GROUP:
      return "Push group";
   case GL_DEBUG_TYPE_POP_GROUP:
      return "Pop group";
   case GL_DEBUG_TYPE_OTHER:
      return "Other";
   default:
      return "";
   }
}


std::string severityToString(GLenum severity)
{
   switch (severity)
   {
   case GL_DEBUG_SEVERITY_HIGH:
      return "High";
   case GL_DEBUG_SEVERITY_MEDIUM:
      return "Medium";
   case GL_DEBUG_SEVERITY_LOW:
      return "Low";
   case GL_DEBUG_SEVERITY_NOTIFICATION:
      return "Notification";
   default:
      return "";
   }
}

} // namespace


namespace gll
{
///////////////////

bool haveDebugContext()
{
   int flags = 0;
   glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
   return flags & GL_CONTEXT_FLAG_DEBUG_BIT;
}


bool setDebugOutputCallback(DebugCallback callback)
{
   if (!haveDebugContext())
      return false;

   glEnable(GL_DEBUG_OUTPUT);
   glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
   glDebugMessageCallback(callback ? callback : logDebugInfo, nullptr);
   glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

   return true;
}


void logDebugInfo(GLenum source, GLenum type, unsigned int id, GLenum severity,
                  GLsizei /*length*/, const char* message, const void* /*userParam*/)
{
   std::string log = "[OpenGL Debug] - ";
   log += "[Message id]: " + std::to_string(id);
   log += ", [Message]: " + std::string{message};
   log += ", [Source]: " + sourceToString(source);
   log += ", [Type]: " + typeToString(type);
   log += ", [Severity]: " + severityToString(severity);
   log += "\n";

   std::cout << log;
}

} // namespace gll
