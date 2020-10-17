//
// Oct-2020, Michael Lindner
// MIT license
//
#include "gl_vertex_array.h"
#include <cassert>


namespace glutil
{
///////////////////

VertexArray::VertexArray(GlId id)
   : m_id{id}
{
}


VertexArray::~VertexArray()
{
   destroy();
}


VertexArray::VertexArray(VertexArray&& other)
{
   swap(*this, other);
}


VertexArray& VertexArray::operator=(VertexArray&& other)
{
   destroy();
   swap(*this, other);
   return *this;
}


bool VertexArray::create()
{
   assert(m_id == 0);
   if (m_id == 0)
      glGenVertexArrays(1, &m_id);
   return m_id != 0;
}


void VertexArray::destroy()
{
   if (m_id != 0)
   {
      glDeleteVertexArrays(1, &m_id);
      m_id = 0;
   }
}


void VertexArray::attach(GlId id)
{
   destroy();
   m_id = id;
}


GlId VertexArray::detach()
{
   GlId id = m_id;
   m_id = 0;
   return id;
}


void VertexArray::bind()
{
   if (m_id != 0)
      glBindVertexArray(m_id);
}


void VertexArray::unbind()
{
   glBindVertexArray(0);
}

} // namespace glutil
