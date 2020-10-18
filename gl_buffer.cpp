//
// Oct-2020, Michael Lindner
// MIT license
//
#include "gl_buffer.h"
#include <cassert>


namespace glutil
{
///////////////////

void Buffer::bind(GLenum target)
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


GlId Buffer::create_()
{
   GlId id;
   glGenBuffers(1, &id);
   return id;
}


void Buffer::destroy_(GlId id)
{
   glDeleteBuffers(1, &id);
}

} // namespace glutil
