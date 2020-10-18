//
// Oct-2020, Michael Lindner
// MIT license
//
#include "gl_vertex_array.h"
#include "gl_data_format.h"
#include <cassert>


namespace glutil
{
///////////////////

VertexArray::VertexArray(GlId id) : m_id{id}
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


// Operates on currently bound vao.
void VertexArray::unbind()
{
   glBindVertexArray(0);
}


// Operates on currently bound vao.
void VertexArray::setAttribFormat(GLuint attribIdx, const DataFormat& format)
{
   glVertexAttribPointer(attribIdx, format.size, format.type, format.normalized,
                         format.stride, format.pointer);
}


// Operates on currently bound vao.
void VertexArray::setAttribIFormat(GLuint attribIdx, const DataFormat& format)
{
   glVertexAttribIPointer(attribIdx, format.size, format.type, format.stride,
                          format.pointer);
}


// Operates on currently bound vao.
void VertexArray::setAttribLFormat(GLuint attribIdx, const DataFormat& format)
{
   glVertexAttribLPointer(attribIdx, format.size, format.type, format.stride,
                          format.pointer);
}


// Operates on currently bound vao.
void VertexArray::enableAttrib(GLuint attribIdx)
{
   glEnableVertexAttribArray(attribIdx);
}


// Operates on currently bound vao.
void VertexArray::disableAttrib(GLuint attribIdx)
{
   glDisableVertexAttribArray(attribIdx);
}

} // namespace glutil
