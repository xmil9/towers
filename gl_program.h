//
// Oct-2020, Michael Lindner
// MIT license
//
#pragma once
#include "glad/glad.h" // glad must be included before anything else opengl related.
#include "gl_object.h"
#include "gl_types.h"
#include "gl_uniform.h"
#include <string>
#include <utility>

namespace glutil { class Shader; }


namespace glutil
{
///////////////////

class Program : public Object<Program>
{
   friend class Object<Program>;

 public:
   Program() = default;
   explicit Program(GlId id);
   ~Program() = default;
   Program(const Program&) = delete;
   Program(Program&& other) = default;

   Program& operator=(const Program&) = delete;
   Program& operator=(Program&& other) = default;

   void attachShader(const Shader& shader);
   void detachShader(const Shader& shader);
   bool link();
   std::string linkLog() const;
   void use();
   // The program must be successully linked to access uniforms.
   Uniform uniform(const GLchar *name) const;

   friend inline void swap(Program& a, Program& b)
   {
      swap(static_cast<Object<Program>&>(a), static_cast<Object<Program>&>(b));
   }

 private:
   // Interface required by Object.
   GlId create_();
   void destroy_(GlId id);
};


inline Program::Program(GlId id) : Object<Program>{id}
{
}

} // namespace glutil
