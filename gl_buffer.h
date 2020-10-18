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

class Buffer : public Object<Buffer>
{
   friend class Object<Buffer>;

 public:
   Buffer() = default;
   explicit Buffer(GlId id);
   ~Buffer() = default;
   Buffer(const Buffer&) = delete;
   Buffer(Buffer&& other) = default;

   Buffer& operator=(const Buffer&) = delete;
   Buffer& operator=(Buffer&& other) = default;

   void bind(GLenum target);
   // Unbinds currently bound buffer for the given target.
   static void unbind(GLenum target);
   void setData(GLenum target, GLsizeiptr size, const void* data, GLenum usage);

   friend inline void swap(Buffer& a, Buffer& b)
   {
      swap(static_cast<Object<Buffer>&>(a), static_cast<Object<Buffer>&>(b));
   }

 private:
   // Interface required by Object.
   GlId create_();
   void destroy_(GlId id);
};


inline Buffer::Buffer(GlId id) : Object<Buffer>{id}
{
}

} // namespace glutil
