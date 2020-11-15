//
// Oct-2020, Michael Lindner
// MIT license
//
#pragma once
#include "glad/glad.h" // glad must be included before anything else opengl related.
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
class VertexArray : public Object<VertexArray>
{
   friend class Object<VertexArray>;

 public:
   VertexArray() = default;
   explicit VertexArray(ObjId id);
   ~VertexArray() = default;
   VertexArray(const VertexArray&) = delete;
   VertexArray(VertexArray&& other) = default;

   VertexArray& operator=(const VertexArray&) = delete;
   VertexArray& operator=(VertexArray&& other) = default;

   void bind() const;
   static void unbind();
   static void setAttribFormat(GLuint attribIdx, const DataFormat& format);
   static void setAttribIFormat(GLuint attribIdx, const DataFormat& format);
   static void setAttribLFormat(GLuint attribIdx, const DataFormat& format);
   static void enableAttrib(GLuint attribIdx);
   static void disableAttrib(GLuint attribIdx);

   friend inline void swap(VertexArray& a, VertexArray& b)
   {
      swap(static_cast<Object<VertexArray>&>(a), static_cast<Object<VertexArray>&>(b));
   }

 private:
   // Interface required by Object.
   ObjId create_();
   void destroy_(ObjId id);
};


inline VertexArray::VertexArray(ObjId id) : Object<VertexArray>{id}
{
}

} // namespace gll
