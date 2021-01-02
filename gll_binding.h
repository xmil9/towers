//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "gll_vbo.h"
#include <utility>

namespace gll
{
struct DataFormat;
}


namespace gll
{
///////////////////

// Unbinds active GL object type at the end of the lifetime of the RAII object.
// Requirements for T:
// - Has member function bind().
// - Has static member function unbind().
template <typename T> struct BindingScope
{
   BindingScope() = default;
   explicit BindingScope(const T& bound);
   ~BindingScope();
   BindingScope(const BindingScope&) = delete;
   BindingScope(BindingScope&& other);
   BindingScope& operator=(const BindingScope&) = delete;
   BindingScope& operator=(BindingScope&& other);

   void bind(const T& bound);
   void unbind();

   friend inline void swap(BindingScope& a, BindingScope& b)
   {
      std::swap(a.m_isBound, b.m_isBound);
   }

 private:
   bool m_isBound = false;
};


template <typename T> BindingScope<T>::BindingScope(const T& bound)
{
   bind(bound);
}

template <typename T> BindingScope<T>::~BindingScope()
{
   unbind();
}

template <typename T> BindingScope<T>::BindingScope(BindingScope&& other)
{
   swap(*this, other);
}

template <typename T> BindingScope<T>& BindingScope<T>::operator=(BindingScope&& other)
{
   unbind();
   swap(*this, other);
}

template <typename T> void BindingScope<T>::bind(const T& bound)
{
   unbind();
   m_isBound = true;
   bound.bind();
}

template <typename T> void BindingScope<T>::unbind()
{
   if (m_isBound)
   {
      T::unbind();
      m_isBound = false;
   }
}


///////////////////

// Unbinds active vbo of given target type at the end of the lifetime of the RAII object.
class VboBindingScope
{
 public:
   VboBindingScope() = default;
   VboBindingScope(const Vbo& bound, GLenum target);
   ~VboBindingScope() { unbind(); }
   VboBindingScope(const VboBindingScope&) = delete;
   VboBindingScope(VboBindingScope&& other) noexcept;
   VboBindingScope& operator=(const VboBindingScope&) = delete;
   VboBindingScope& operator=(VboBindingScope&& other) noexcept;

   void bind(const Vbo& bound, GLenum target);
   void unbind();

   friend inline void swap(VboBindingScope& a, VboBindingScope& b)
   {
      std::swap(a.m_target, b.m_target);
   }

 private:
   GLenum m_target = 0;
};


inline VboBindingScope::VboBindingScope(const Vbo& bound, GLenum target)
: m_target{target}
{
   bind(bound, m_target);
}

inline VboBindingScope::VboBindingScope(VboBindingScope&& other) noexcept
{
   swap(*this, other);
}

inline VboBindingScope& VboBindingScope::operator=(VboBindingScope&& other) noexcept
{
   unbind();
   swap(*this, other);
}

inline void VboBindingScope::bind(const Vbo& bound, GLenum target)
{
   unbind();
   m_target = target;
   bound.bind(m_target);
}

inline void VboBindingScope::unbind()
{
   if (m_target != 0)
   {
      Vbo::unbind(m_target);
      m_target = 0;
   }
}


///////////////////

// Combines a vbo and its binding scope.
// Helper class for vbo binding functions below.
class BoundVbo
{
 public:
   bool create() { return m_vbo.create(); }
   void bind(GLenum target) { m_binding.bind(m_vbo, target); }
   void setData(GLenum target, GLsizeiptr size, const void* data, GLenum usage);

 private:
   gll::Vbo m_vbo;
   gll::VboBindingScope m_binding;
};


inline void BoundVbo::setData(GLenum target, GLsizeiptr size, const void* data,
                              GLenum usage)
{
   m_vbo.setData(target, size, data, usage);
}


///////////////////

// Binds array vbo to current vao.
void bindArrayVbo(GLuint attribIdx, const void* data, std::size_t dataSize,
                  const gll::DataFormat& format, BoundVbo& buf);

// Binds element vbo to current vao.
void bindElementVbo(const void* data, std::size_t dataSize, BoundVbo& buf);

} // namespace gll
