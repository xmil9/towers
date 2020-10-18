//
// Oct-2020, Michael Lindner
// MIT license
//
#pragma once
#include "glad/glad.h" // glad must be included before anything else opengl related.
#include "gl_types.h"
#include <utility>


namespace glutil
{
///////////////////

class Buffer
{
 public:
   Buffer() = default;
   explicit Buffer(GlId id);
   ~Buffer();
   Buffer(const Buffer&) = delete;
   Buffer(Buffer&& other);

   Buffer& operator=(const Buffer&) = delete;
   Buffer& operator=(Buffer&& other);
   explicit operator bool() const { return m_id != 0; }
   bool operator!() const { return !operator bool(); }

   GlId id() const { return m_id; }
   bool create();
   void destroy();
   void attach(GlId id);
   GlId detach();
   void bind(GLenum target);
   // Unbinds currently bound buffer for the given target.
   static void unbind(GLenum target);
   void setData(GLenum target, GLsizeiptr size, const void* data, GLenum usage);

   friend inline void swap(Buffer& a, Buffer& b) { std::swap(a.m_id, b.m_id); }

 private:
   GlId m_id = 0;
};

} // namespace glutil
