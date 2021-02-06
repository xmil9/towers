//
// Oct-2020, Michael Lindner
// MIT license
//
#pragma once
#include "glad/glad.h" // glad must be included before anything else opengl related.
#include "gll_object.h"
#include "gll_uniform.h"
#include <string>
#include <utility>

namespace gll
{
class Shader;
}


namespace gll
{
///////////////////

class Program : public Object<Program>
{
   friend class Object<Program>;

 public:
   Program() = default;
   explicit Program(ObjId id);
   ~Program() = default;
   Program(const Program&) = delete;
   Program(Program&& other) noexcept = default;

   Program& operator=(const Program&) = delete;
   Program& operator=(Program&& other) noexcept = default;

   void attachShader(const Shader& shader);
   void detachShader(const Shader& shader);
   bool link();
   std::string linkLog() const;
   void use() const;
   // The program must be successully linked to access uniforms.
   Uniform uniform(const GLchar* name) const;
   // Creates a uniform with a given name and value.
   template<typename Value>
   void setUniform(const GLchar* name, const Value& v);
   // Set the texture sampler to use a texture bound to the unit with the given index.
   // Program must be in-use.
   void setTextureUnit(const GLchar* samplerName, GLint unitIdx) const;

   friend inline void swap(Program& a, Program& b)
   {
      swap(static_cast<Object<Program>&>(a), static_cast<Object<Program>&>(b));
   }

 private:
   // Interface required by Object.
   ObjId create_();
   void destroy_(ObjId id);
};


inline Program::Program(ObjId id) : Object<Program>{id}
{
}

template<typename Value>
void Program::setUniform(const GLchar* name, const Value& v)
{
   gll::Uniform u = uniform(name);
   u.setValue(v);
}

} // namespace gll
