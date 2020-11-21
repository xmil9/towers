//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "gll_vbo.h"
#include <utility>


namespace gll
{
///////////////////

// RAII helper to unbind OpenGL objects at end of their lifetime.
// Requirements for T:
// - Has member function bind().
// - Has static member function unbind().
template <typename T> struct Binding
{
   Binding() {}
   explicit Binding(const T& bound);
   ~Binding();
   Binding(const Binding&) = delete;
   Binding(Binding&& other);
   Binding& operator=(const Binding&) = delete;
   Binding& operator=(Binding&& other);

   void bind(const T& bound);
   void unbind();

   friend inline void swap(Binding& a, Binding& b)
   {
      std::swap(a.m_isBound, b.m_isBound);
   }

 private:
   bool m_isBound = false;
};


template <typename T>
Binding<T>::Binding(const T& bound)
{
   bind(bound);
}

template <typename T>
Binding<T>::~Binding()
{
   unbind();
}

template <typename T>
Binding<T>::Binding(Binding&& other)
{
   swap(*this, other);
}

template <typename T>
Binding<T>& Binding<T>::operator=(Binding&& other)
{
   unbind();
   swap(*this, other);
}

template <typename T>
void Binding<T>::bind(const T& bound)
{
   unbind();
   m_isBound = true;
   bound.bind();
}

template <typename T>
void Binding<T>::unbind()
{
   if (m_isBound)
   {
      T::unbind();
      m_isBound = false;
   }
}


///////////////////

// RAII helper to unbind vbos at end of their lifetime.
class VboBinding
{
public:
   VboBinding() = default;
   explicit VboBinding(const Vbo& bound, GLenum target);
   ~VboBinding() { unbind(); }
   VboBinding(const VboBinding&) = delete;
   VboBinding(VboBinding&& other);
   VboBinding& operator=(const VboBinding&) = delete;
   VboBinding& operator=(VboBinding&& other);

   void bind(const Vbo& bound, GLenum target);
   void unbind();

   friend inline void swap(VboBinding& a, VboBinding& b)
   {
      std::swap(a.m_target, b.m_target);
   }

 private:
   GLenum m_target = 0;
};


inline VboBinding::VboBinding(const Vbo& bound, GLenum target) : m_target{target}
{
   bind(bound, m_target);
}

inline VboBinding::VboBinding(VboBinding&& other)
{
   swap(*this, other);
}

inline VboBinding& VboBinding::operator=(VboBinding&& other)
{
   unbind();
   swap(*this, other);
}

inline void VboBinding::bind(const Vbo& bound, GLenum target)
{
   unbind();
   m_target = target;
   bound.bind(m_target);
}

inline void VboBinding::unbind()
{
   if (m_target != 0)
   {
      Vbo::unbind(m_target);
      m_target = 0;
   }
}

} // namespace gll
