//
// Oct-2020, Michael Lindner
// MIT license
//
#pragma once
#include "glad/glad.h" // glad must be included before anything else opengl related.
#include "gl_types.h"
#include <utility>

namespace glutil { struct DataFormat; }


namespace glutil
{
///////////////////

// Vertex arrays serve as a collection of attributes that describe vertices,
// e.g. position, normal, color/texture, etc. Each attribute consists of
// the data values given in a buffer (e.g. vbo) bound to the vertex array
// and the format description of the data.
// Vertex array are useful to group all data needed to render shapes and
// to active/deactivate it collectively.
// Static member functions operate on the currently bound vertex array.
class VertexArray
{
 public:
   VertexArray() = default;
   explicit VertexArray(GlId id);
   ~VertexArray();
   VertexArray(const VertexArray&) = delete;
   VertexArray(VertexArray&& other);

   VertexArray& operator=(const VertexArray&) = delete;
   VertexArray& operator=(VertexArray&& other);
   explicit operator bool() const { return m_id != 0; }
   bool operator!() const { return !operator bool(); }

   GlId id() const { return m_id; }
   bool create();
   void destroy();
   void attach(GlId id);
   GlId detach();
   void bind();
   static void unbind();
   static void setAttribFormat(GLuint attribIdx, const DataFormat& format);
   static void setAttribIFormat(GLuint attribIdx, const DataFormat& format);
   static void setAttribLFormat(GLuint attribIdx, const DataFormat& format);
   static void enableAttrib(GLuint attribIdx);
   static void disableAttrib(GLuint attribIdx);

   friend inline void swap(VertexArray& a, VertexArray& b) { std::swap(a.m_id, b.m_id); }

 private:
   GlId m_id = 0;
};

} // namespace glutil
