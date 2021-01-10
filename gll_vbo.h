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

class Vbo : public Object<Vbo>
{
   friend class Object<Vbo>;

 public:
   Vbo() = default;
   explicit Vbo(ObjId id);
   ~Vbo() = default;
   Vbo(const Vbo&) = delete;
   Vbo(Vbo&& other) noexcept = default;

   Vbo& operator=(const Vbo&) = delete;
   Vbo& operator=(Vbo&& other) noexcept = default;

   void bind(GLenum target) const;
   // Unbinds currently bound vbo for the given target.
   static void unbind(GLenum target);
   // Vbo needs to be active/bound in order to set data.
   void setData(GLenum target, GLsizeiptr size, const void* data, GLenum usage);
   void setSubData(GLenum target, GLintptr offset, GLsizeiptr size, const void* data);

   friend inline void swap(Vbo& a, Vbo& b)
   {
      swap(static_cast<Object<Vbo>&>(a), static_cast<Object<Vbo>&>(b));
   }

 private:
   // Interface required by Object.
   ObjId create_();
   void destroy_(ObjId id);
};


inline Vbo::Vbo(ObjId id) : Object<Vbo>{id}
{
}

inline void Vbo::bind(GLenum target) const
{
   if (hasId())
      glBindBuffer(target, id());
}

inline void Vbo::unbind(GLenum target)
{
   glBindBuffer(target, 0);
}

inline void Vbo::setData(GLenum target, GLsizeiptr size, const void* data, GLenum usage)
{
   if (hasId())
      glBufferData(target, size, data, usage);
}

inline void Vbo::setSubData(GLenum target, GLintptr offset, GLsizeiptr size,
                            const void* data)
{
   if (hasId())
      glBufferSubData(target, offset, size, data);
}

inline ObjId Vbo::create_()
{
   ObjId id;
   glGenBuffers(1, &id);
   return id;
}

inline void Vbo::destroy_(ObjId id)
{
   glDeleteBuffers(1, &id);
}

} // namespace gll
