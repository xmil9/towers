//
// Oct-2020, Michael Lindner
// MIT license
//
#pragma once
#include "glad/glad.h" // glad must be included before anything else opengl related.


namespace gll
{
///////////////////

struct DataFormat
{
   // Number of components per attribute, e.g. for 3D positions the size would be 3
   // because there are 3 components (x, y, z).
   GLint size = 4;

   // Data type of each component, e.g. GL_FLOAT.
   GLenum type = GL_FLOAT;

   // GL_TRUE to normalize integer values to [-1, 1] for signed values or [0, 1] for
   // unsigned values.
   // GL_FALSE to not normalize the values.
   GLboolean normalized = GL_FALSE;

   // Offset between attributes, e.g. 3 * sizeof(float) for 3D positions with float
   // coordinates. 0 means attributes are tightly packed, i.e. no padding.
   GLsizei stride = 0;

   // Offset to first attribute.
   const void* pointer = nullptr;
};

} // namespace gll
