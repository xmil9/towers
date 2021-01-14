//
// Nov-2020, Michael Lindner
// MIT license
//
#include "gll_binding.h"
#include "gll_vao.h"


namespace gll
{
///////////////////

void bindArrayVbo(gll::Vbo& vbo, GLuint attribIdx, const void* data, std::size_t dataSize,
                  const gll::DataFormat& format, GLenum usage)
{
   vbo.bind(GL_ARRAY_BUFFER);
   vbo.setData(GL_ARRAY_BUFFER, dataSize, data, usage);

   Vao::setAttribFormat(attribIdx, format);
   Vao::enableAttrib(attribIdx);
}


void bindElementVbo(gll::Vbo& vbo, const void* data, std::size_t dataSize, GLenum usage)
{
   vbo.bind(GL_ELEMENT_ARRAY_BUFFER);
   vbo.setData(GL_ELEMENT_ARRAY_BUFFER, dataSize, data, usage);
}

} // namespace gll
