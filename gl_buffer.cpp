//
// Oct-2020, Michael Lindner
// MIT license
//
#include "gl_buffer.h"
#include <cassert>


namespace glutil
{
///////////////////

Buffer::Buffer(GlId id) : m_id{id}
{
}


Buffer::~Buffer()
{
   destroy();
}


Buffer::Buffer(Buffer&& other)
{
   swap(*this, other);
}


Buffer& Buffer::operator=(Buffer&& other)
{
   destroy();
   swap(*this, other);
   return *this;
}


bool Buffer::create()
{
   assert(m_id == 0);
   if (m_id == 0)
      glGenBuffers(1, &m_id);
   return m_id != 0;
}


void Buffer::destroy()
{
   if (m_id != 0)
   {
      glDeleteBuffers(1, &m_id);
      m_id = 0;
   }
}


void Buffer::attach(GlId id)
{
   destroy();
   m_id = id;
}


GlId Buffer::detach()
{
   GlId id = m_id;
   m_id = 0;
   return id;
}


void Buffer::bind(GLenum target)
{
   if (m_id != 0)
      glBindBuffer(target, m_id);
}


void Buffer::unbind(GLenum target)
{
   glBindBuffer(target, 0);
}


void Buffer::setData(GLenum target, GLsizeiptr size, const void* data, GLenum usage)
{
   if (m_id != 0)
      glBufferData(target, size, data, usage);
}

} // namespace glutil
