//
// Oct-2020, Michael Lindner
// MIT license
//
#include "gl_vertex_array.h"
#include "gl_data_format.h"


namespace glutil
{
///////////////////

void VertexArray::bind()
{
   if (hasId())
      glBindVertexArray(id());
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


GlId VertexArray::create_()
{
   GlId id;
   glGenVertexArrays(1, &id);
   return id;
}


void VertexArray::destroy_(GlId id)
{
   glDeleteVertexArrays(1, &id);
}

} // namespace glutil
