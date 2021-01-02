//
// Oct-2020, Michael Lindner
// MIT license
//
#pragma once
#include "glad/glad.h" // glad must be included before anything else opengl related.
#include "gll_types.h"
#include <cassert>
#include <type_traits>


namespace gll
{
///////////////////

// Base class for OpenGL objects. Manages creation and deletion of objects.
// Uses CRTP.
// Derived classes have to implement:
// - ObjId create() member function
// - void destroy(ObjId id) member function
// - void swap(Derived& a, Derived& b) free function
template <typename Derived> class Object
{
 public:
   ~Object();

   explicit operator bool() const { return hasId(); }
   bool operator!() const { return !operator bool(); }

   bool hasId() const { return m_id != 0; }
   ObjId id() const { return m_id; }
   bool create();
   void destroy();
   void attach(ObjId id);
   ObjId detach();

 protected:
   // Only allowed to be used through derivation.
   Object() = default;
   Object(ObjId id);
   Object(const Object&) = delete;
   Object(Object&& other) noexcept;

   Object& operator=(const Object&) = delete;
   Object& operator=(Object&& other) noexcept;

   void setId(ObjId id) { m_id = id; }
   friend inline void swap(Object& a, Object& b) { std::swap(a.m_id, b.m_id); }

 private:
   Derived& GetDerived() { return static_cast<Derived&>(*this); }
   const Derived& GetDerived() const { return static_cast<const Derived&>(*this); }

 private:
   ObjId m_id = 0;
};


template <typename Derived> Object<Derived>::Object(ObjId id) : m_id{id}
{
}


template <typename Derived> Object<Derived>::~Object()
{
   destroy();
}


template <typename Derived> Object<Derived>::Object(Object&& other) noexcept
{
   swap(GetDerived(), other.GetDerived());
}


template <typename Derived>
Object<Derived>& Object<Derived>::operator=(Object&& other) noexcept
{
   destroy();
   swap(GetDerived(), other.GetDerived());
   return *this;
}


template <typename Derived> bool Object<Derived>::create()
{
   assert(!hasId());
   if (!hasId())
      m_id = GetDerived().create_();
   return hasId();
}


template <typename Derived> void Object<Derived>::destroy()
{
   if (hasId())
   {
      GetDerived().destroy_(m_id);
      m_id = 0;
   }
}


template <typename Derived> void Object<Derived>::attach(ObjId id)
{
   destroy();
   m_id = id;
}


template <typename Derived> ObjId Object<Derived>::detach()
{
   ObjId id = m_id;
   m_id = 0;
   return id;
}

} // namespace gll
