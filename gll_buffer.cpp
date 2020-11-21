//
// Oct-2020, Michael Lindner
// MIT license
//
#include "gll_buffer.h"


namespace gll
{
///////////////////

void Buffer::bind(GLenum target) const
{
   if (hasId())
      glBindBuffer(target, id());
}


void Buffer::unbind(GLenum target)
{
   glBindBuffer(target, 0);
}


void Buffer::setData(GLenum target, GLsizeiptr size, const void* data, GLenum usage)
{
   if (hasId())
      glBufferData(target, size, data, usage);
}


ObjId Buffer::create_()
{
   ObjId id;
   glGenBuffers(1, &id);
   return id;
}


void Buffer::destroy_(ObjId id)
{
   glDeleteBuffers(1, &id);
}

} // namespace gll
