//
// Oct-2020, Michael Lindner
// MIT license
//
#pragma once
#include "glad/glad.h" // glad must be included before anything else opengl related.
#include "gl_types.h"
#include <cassert>
#include <type_traits>


namespace glutil
{
///////////////////

// Base class for OpenGL objects. Manages creation and deletion of objects.
// Uses CRTP.
// Derived classes have to implement:
// - GlId create() member function
// - void destroy(GlId id) member function
// - void swap(Derived& a, Derived& b) free function
template <typename GlType> class Object
{
 public:
   ~Object();

   explicit operator bool() const { return hasId(); }
   bool operator!() const { return !operator bool(); }

   bool hasId() const { return m_id != 0; }
   GlId id() const { return m_id; }
   bool create();
   void destroy();
   void attach(GlId id);
   GlId detach();

 protected:
   // Only allowed to be used through derivation.
   Object() = default;
   Object(GlId id);
   Object(const Object&) = delete;
   Object(Object&& other);

   Object& operator=(const Object&) = delete;
   Object& operator=(Object&& other);

   void setId(GlId id) { m_id = id; }
   friend inline void swap(Object& a, Object& b) { std::swap(a.m_id, b.m_id); }

 private:
   GlType& GetGlType() { return static_cast<GlType&>(*this); }
   const GlType& GetGlType() const { return static_cast<const GlType&>(*this); }

 private:
   GlId m_id = 0;
};


template <typename GlType> Object<GlType>::Object(GlId id) : m_id{id}
{
}


template <typename GlType> Object<GlType>::~Object()
{
   destroy();
}


template <typename GlType> Object<GlType>::Object(Object&& other)
{
   swap(GetGlType(), other.GetGlType());
}


template <typename GlType> Object<GlType>& Object<GlType>::operator=(Object&& other)
{
   destroy();
   swap(GetGlType(), other.GetGlType());
   return *this;
}


template <typename GlType> bool Object<GlType>::create()
{
   assert(!hasId());
   if (!hasId())
      m_id = GetGlType().create_();
   return hasId();
}


template <typename GlType> void Object<GlType>::destroy()
{
   if (hasId())
   {
      GetGlType().destroy_(m_id);
      m_id = 0;
   }
}


template <typename GlType> void Object<GlType>::attach(GlId id)
{
   destroy();
   m_id = id;
}


template <typename GlType> GlId Object<GlType>::detach()
{
   GlId id = m_id;
   m_id = 0;
   return id;
}

} // namespace glutil
