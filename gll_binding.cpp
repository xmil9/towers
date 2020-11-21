//
// Nov-2020, Michael Lindner
// MIT license
//
#include "gll_binding.h"
#include "gll_vao.h"


namespace gll
{
///////////////////

void bindArrayVbo(GLuint attribIdx, const void* data, std::size_t dataSize,
                  const gll::DataFormat& format, BoundVbo& buf)
{
   if (dataSize == 0)
      return;

   buf.create();
   buf.bind(GL_ARRAY_BUFFER);
   buf.setData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);

   Vao::setAttribFormat(attribIdx, format);
   Vao::enableAttrib(attribIdx);
}


void bindElementVbo(const void* data, std::size_t dataSize, BoundVbo& buf)
{
   buf.create();
   buf.bind(GL_ELEMENT_ARRAY_BUFFER);
   buf.setData(GL_ELEMENT_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
}

} // namespace gll
