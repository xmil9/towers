//
// Oct-2020, Michael Lindner
// MIT license
//
#pragma once
#include "glad/glad.h" // glad must be included before anything else opengl related.
#include "gll_object.h"
#include <filesystem>


namespace gll
{
///////////////////

class Texture2D : public Object<Texture2D>
{
   friend class Object<Texture2D>;

 public:
   Texture2D() = default;
   explicit Texture2D(ObjId id);
   ~Texture2D() = default;
   Texture2D(const Texture2D&) = delete;
   Texture2D(Texture2D&& other) noexcept = default;

   Texture2D& operator=(const Texture2D&) = delete;
   Texture2D& operator=(Texture2D&& other) noexcept = default;

   void bind() const;
   // Unbinds currently bound texture for the given target.
   static void unbind();
   // Buffer needs to be active/bound in order to set data.
   void setData(GLint level, GLint internalFormat, GLsizei width, GLsizei height,
                GLenum format, GLenum type, const void* data);
   // Loads texture from file and then sets the data.
   bool loadData(const std::filesystem::path& texFile, bool flipVert, GLint level,
                 GLint internalFormat, GLenum format, GLenum type);
   void generateMipmap();
   // Sets wrapping policy for x- and y-direction:
   // - GL_REPEAT (default), GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER
   // - '-1' for unchanged (when only changing one option)
   void setWrapPolicy(GLint xPolicy, GLint yPolicy);
   // Sets filtering for scaling the texture:
   // - GL_NEAREST (default), GL_LINEAR
   // - '-1' for unchanged (when only changing one option)
   void setScaleFiltering(GLint minifyFilter, GLint magnifyFilter);

   friend inline void swap(Texture2D& a, Texture2D& b)
   {
      swap(static_cast<Object<Texture2D>&>(a), static_cast<Object<Texture2D>&>(b));
   }

 private:
   // Interface required by Object.
   ObjId create_();
   void destroy_(ObjId id);
};


inline Texture2D::Texture2D(ObjId id) : Object<Texture2D>{id}
{
}

} // namespace gll
