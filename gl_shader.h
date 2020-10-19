//
// Oct-2020, Michael Lindner
// MIT license
//
#pragma once
#include "glad/glad.h" // glad must be included before anything else opengl related.
#include "gl_object.h"
#include "gl_types.h"
#include <utility>


namespace glutil
{
///////////////////

class Shader : public Object<Shader>
{
   friend class Object<Shader>;

 public:
   Shader() = default;
   explicit Shader(GlId id);
   ~Shader() = default;
   Shader(const Shader&) = delete;
   Shader(Shader&& other) = default;

   Shader& operator=(const Shader&) = delete;
   Shader& operator=(Shader&& other) = default;

   bool create(GLenum shaderType);
   bool compile();
   void setSource(const GLchar* code) { setSource(1, &code, nullptr); }
   void setSource(GLsizei count, const GLchar** code, const GLint* length);

   friend inline void swap(Shader& a, Shader& b)
   {
      swap(static_cast<Object<Shader>&>(a), static_cast<Object<Shader>&>(b));
   }

 private:
   // Hide from public access because a special overload should be used instead.
   using Object<Shader>::create;

 private:
   // Interface required by Object.
   // Note that the other required member function create_() will never be called
   // because a special overload for creating shader objects is used.
   void destroy_(GlId id);
};


inline Shader::Shader(GlId id) : Object<Shader>{id}
{
}


///////////////////

Shader makeVertexShader(const GLchar* code);
Shader makeFragmentShader(const GLchar* code);

} // namespace glutil
