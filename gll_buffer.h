//
// Oct-2020, Michael Lindner
// MIT license
//
#pragma once
#include "glad/glad.h" // glad must be included before anything else opengl related.
#include "gll_object.h"


namespace gll
{
///////////////////

class Buffer : public Object<Buffer>
{
   friend class Object<Buffer>;

 public:
   Buffer() = default;
   explicit Buffer(ObjId id);
   ~Buffer() = default;
   Buffer(const Buffer&) = delete;
   Buffer(Buffer&& other) = default;

   Buffer& operator=(const Buffer&) = delete;
   Buffer& operator=(Buffer&& other) = default;

   void bind(GLenum target) const;
   // Unbinds currently bound buffer for the given target.
   static void unbind(GLenum target);
   // Buffer needs to be active/bound in order to set data.
   void setData(GLenum target, GLsizeiptr size, const void* data, GLenum usage);

   friend inline void swap(Buffer& a, Buffer& b)
   {
      swap(static_cast<Object<Buffer>&>(a), static_cast<Object<Buffer>&>(b));
   }

 private:
   // Interface required by Object.
   ObjId create_();
   void destroy_(ObjId id);
};


inline Buffer::Buffer(ObjId id) : Object<Buffer>{id}
{
}

} // namespace gll
