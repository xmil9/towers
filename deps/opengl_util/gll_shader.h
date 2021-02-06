//
// Oct-2020, Michael Lindner
// MIT license
//
#pragma once
#include "glad/glad.h" // glad must be included before anything else opengl related.
#include "gll_object.h"
#include <filesystem>
#include <string>
#include <utility>


namespace gll
{
///////////////////

class Shader : public Object<Shader>
{
   friend class Object<Shader>;

 public:
   Shader() = default;
   explicit Shader(ObjId id);
   ~Shader() = default;
   Shader(const Shader&) = delete;
   Shader(Shader&& other) noexcept = default;

   Shader& operator=(const Shader&) = delete;
   Shader& operator=(Shader&& other) noexcept = default;

   bool create(GLenum shaderType);
   void setSource(const GLchar* code) { setSource(1, &code, nullptr); }
   void setSource(GLsizei count, const GLchar** code, const GLint* length);
   bool loadSource(const std::filesystem::path& path);
   bool compile();
   std::string compileLog() const;

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
   void destroy_(ObjId id);
};


inline Shader::Shader(ObjId id) : Object<Shader>{id}
{
}


///////////////////

Shader makeVertexShader(const GLchar* code);
Shader makeVertexShader(const std::filesystem::path& codeFile);
Shader makeFragmentShader(const GLchar* code);
Shader makeFragmentShader(const std::filesystem::path& codeFile);

} // namespace gll
