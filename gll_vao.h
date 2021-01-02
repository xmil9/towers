//
// Oct-2020, Michael Lindner
// MIT license
//
#pragma once
#include "glad/glad.h" // glad must be included before anything else opengl related.
#include "gll_data_format.h"
#include "gll_object.h"
#include <utility>

namespace gll
{
struct DataFormat;
}


namespace gll
{
///////////////////

// Vertex arrays serve as a collection of attributes that describe vertices,
// e.g. position, normal, color/texture, etc. Each attribute consists of
// the data values given in a buffer (e.g. vbo) bound to the vertex array
// and the format description of the data.
// Vertex array are useful to group all data needed to render shapes and
// to active/deactivate it collectively.
// Static member functions operate on the currently bound vertex array.
class Vao : public Object<Vao>
{
   friend class Object<Vao>;

 public:
   Vao() = default;
   explicit Vao(ObjId id);
   ~Vao() = default;
   Vao(const Vao&) = delete;
   Vao(Vao&& other) noexcept = default;

   Vao& operator=(const Vao&) = delete;
   Vao& operator=(Vao&& other) noexcept = default;

   void bind() const;
   static void unbind();
   static void setAttribFormat(GLuint attribIdx, const DataFormat& format);
   static void setAttribIFormat(GLuint attribIdx, const DataFormat& format);
   static void setAttribLFormat(GLuint attribIdx, const DataFormat& format);
   static void enableAttrib(GLuint attribIdx);
   static void disableAttrib(GLuint attribIdx);

   friend inline void swap(Vao& a, Vao& b)
   {
      swap(static_cast<Object<Vao>&>(a), static_cast<Object<Vao>&>(b));
   }

 private:
   // Interface required by Object.
   ObjId create_();
   void destroy_(ObjId id);
};


inline Vao::Vao(ObjId id) : Object<Vao>{id}
{
}

inline void Vao::bind() const
{
   if (hasId())
      glBindVertexArray(id());
}

// Operates on currently bound vao.
inline void Vao::unbind()
{
   glBindVertexArray(0);
}

// Operates on currently bound vao.
inline void Vao::setAttribFormat(GLuint attribIdx, const DataFormat& format)
{
   glVertexAttribPointer(attribIdx, format.size, format.type, format.normalized,
                         format.stride, format.pointer);
}

// Operates on currently bound vao.
inline void Vao::setAttribIFormat(GLuint attribIdx, const DataFormat& format)
{
   glVertexAttribIPointer(attribIdx, format.size, format.type, format.stride,
                          format.pointer);
}

// Operates on currently bound vao.
inline void Vao::setAttribLFormat(GLuint attribIdx, const DataFormat& format)
{
   glVertexAttribLPointer(attribIdx, format.size, format.type, format.stride,
                          format.pointer);
}

// Operates on currently bound vao.
inline void Vao::enableAttrib(GLuint attribIdx)
{
   glEnableVertexAttribArray(attribIdx);
}

// Operates on currently bound vao.
inline void Vao::disableAttrib(GLuint attribIdx)
{
   glDisableVertexAttribArray(attribIdx);
}

inline ObjId Vao::create_()
{
   ObjId id;
   glGenVertexArrays(1, &id);
   return id;
}

inline void Vao::destroy_(ObjId id)
{
   glDeleteVertexArrays(1, &id);
}

} // namespace gll
