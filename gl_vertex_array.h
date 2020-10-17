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
   // Unbinds currently bound vertex array.
   static void unbind();

   friend inline void swap(VertexArray& a, VertexArray& b)
   {
      std::swap(a.m_id, b.m_id);
   }

 private:
   GlId m_id = 0;
};

} // namespace glutil
