//
// Oct-2020, Michael Lindner
// MIT license
//
#pragma once
#include "glad/glad.h" // glad must be included before anything else opengl related.
#include "gll_types.h"
#include "essentutils/type_traits_util.h"
#ifdef HAVE_GLM
#include "glm/gtc/type_ptr.hpp"
#include "glm/matrix.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#endif
#include <array>
#include <type_traits>
#include <utility>

namespace gll
{
///////////////////

// Overloads for getting uniform values.
// They are used as helper functions but could be useful by themselves.

inline void getUniformValues(ObjId program, GLint location, GLsizei count, GLfloat* vals)
{
   glGetnUniformfv(program, location, count * sizeof(GLfloat), vals);
}

inline void getUniformValues(ObjId program, GLint location, GLsizei count, GLdouble* vals)
{
   glGetnUniformdv(program, location, count * sizeof(GLdouble), vals);
}

inline void getUniformValues(ObjId program, GLint location, GLsizei count, GLint* vals)
{
   glGetnUniformiv(program, location, count * sizeof(GLint), vals);
}

inline void getUniformValues(ObjId program, GLint location, GLsizei count, GLuint* vals)
{
   glGetnUniformuiv(program, location, count * sizeof(GLuint), vals);
}


// Sets uniform values.
// Used as helper function but could be useful by itself.
template <typename T> void setUniformValue(GLint location, const T& val)
{
   if constexpr (std::is_same_v<T, GLfloat>)
      glUniform1f(location, val);
   else if constexpr (std::is_same_v<T, GLint> || std::is_same_v<T, bool>)
      glUniform1i(location, val);
   else if constexpr (std::is_same_v<T, GLuint>)
      glUniform1ui(location, val);
   else if constexpr (std::is_same_v<T, GLdouble>)
      glUniform1d(location, val);
   else if constexpr (std::is_same_v<T, glm::vec2>)
      glUniform2f(location, val[0], val[1]);
   else if constexpr (std::is_same_v<T, glm::ivec2> || std::is_same_v<T, glm::bvec2>)
      glUniform2i(location, val[0], val[1]);
   else if constexpr (std::is_same_v<T, glm::uvec2>)
      glUniform2ui(location, val[0], val[1]);
   else if constexpr (std::is_same_v<T, glm::dvec2>)
      glUniform2d(location, val[0], val[1]);
   else if constexpr (std::is_same_v<T, glm::vec3>)
      glUniform3f(location, val[0], val[1], val[2]);
   else if constexpr (std::is_same_v<T, glm::ivec3> || std::is_same_v<T, glm::bvec3>)
      glUniform3i(location, val[0], val[1], val[2]);
   else if constexpr (std::is_same_v<T, glm::uvec3>)
      glUniform3ui(location, val[0], val[1], val[2]);
   else if constexpr (std::is_same_v<T, glm::dvec3>)
      glUniform3d(location, val[0], val[1], val[2]);
   else if constexpr (std::is_same_v<T, glm::vec4>)
      glUniform4f(location, val[0], val[1], val[2], val[3]);
   else if constexpr (std::is_same_v<T, glm::ivec4> || std::is_same_v<T, glm::bvec4>)
      glUniform4i(location, val[0], val[1], val[2], val[3]);
   else if constexpr (std::is_same_v<T, glm::uvec4>)
      glUniform4ui(location, val[0], val[1], val[2], val[3]);
   else if constexpr (std::is_same_v<T, glm::dvec4>)
      glUniform4d(location, val[0], val[1], val[2], val[3]);
   else if constexpr (std::is_same_v<T, glm::mat2>)
      glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(val));
   else if constexpr (std::is_same_v<T, glm::dmat2>)
      glUniformMatrix2dv(location, 1, GL_FALSE, glm::value_ptr(val));
   else if constexpr (std::is_same_v<T, glm::mat3>)
      glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(val));
   else if constexpr (std::is_same_v<T, glm::dmat3>)
      glUniformMatrix3dv(location, 1, GL_FALSE, glm::value_ptr(val));
   else if constexpr (std::is_same_v<T, glm::mat4>)
      glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(val));
   else if constexpr (std::is_same_v<T, glm::dmat4>)
      glUniformMatrix4dv(location, 1, GL_FALSE, glm::value_ptr(val));
   else
      static_assert(false, "Invalid value type.");
}


///////////////////

class Uniform
{
 public:
   Uniform() = default;
   Uniform(ObjId program, GLint location);
   ~Uniform() = default;
   Uniform(const Uniform&) = default;
   Uniform(Uniform&& other) noexcept = default;

   Uniform& operator=(const Uniform&) = default;
   Uniform& operator=(Uniform&& other) noexcept = default;

   ObjId programId() const { return m_program; }
   GLint location() const { return m_location; }
   explicit operator bool() const { return hasLocation(); }
   bool operator!() const { return !operator bool(); }

   // Gets value of uniform.
   // T can be:
   // - float, int unsigned int, double, bool
   // - glm::vec2|3|4, glm::ivec2|3|4, glm::uvec2|3|4, glm::dvec2|3|4, glm::bvec2|3|4
   // - glm::mat2|3|4, glm::dmat2|3|4
   template <typename T> T value() const;
   // Sets value of uniform. Same types as for getting a value.
   // The uniform's program must be active ('use' must have been called on it) in order to
   // set a uniform's value.
   template <typename T> void setValue(const T& val);

   friend inline void swap(Uniform& a, Uniform& b)
   {
      std::swap(a.m_program, b.m_program);
      std::swap(a.m_location, b.m_location);
   }

 private:
   bool hasLocation() const { return m_location != -1; }

 private:
   // The program id is not used for setting values. Value setters operate on the
   // currently active program.
   ObjId m_program = 0;
   GLint m_location = -1;
};


inline Uniform::Uniform(ObjId program, GLint location)
: m_program{program}, m_location{location}
{
}


template <typename T> T Uniform::value() const
{
   if (!hasLocation())
      return {};

#pragma warning(disable : 4201)

   if constexpr (esl::isOneOf_v<T, GLfloat, GLint, GLuint, GLdouble, bool>)
   {
      using GlValue = typename std::conditional<std::is_same_v<T, bool>, GLint, T>::type;

      GlValue val = 0;
      getUniformValues(m_program, m_location, 1, &val);
      return val;
   }
#ifdef HAVE_GLM
   else if constexpr (esl::isOneOf_v<T, glm::vec2, glm::ivec2, glm::uvec2, glm::dvec2,
                                       glm::bvec2>)
   {
      using Value = typename T::value_type;
      using GlValue =
         typename std::conditional<std::is_same_v<Value, bool>, GLint, Value>::type;

      std::array<GlValue, 2> vals;
      getUniformValues(m_program, m_location, 2, vals.data());
      return T(vals[0], vals[1]);
   }
   else if constexpr (esl::isOneOf_v<T, glm::vec3, glm::ivec3, glm::uvec3, glm::dvec3,
                                       glm::bvec3>)
   {
      using Value = typename T::value_type;
      using GlValue =
         typename std::conditional<std::is_same_v<Value, bool>, GLint, Value>::type;

      std::array<GlValue, 3> vals;
      getUniformValues(m_program, m_location, 3, vals.data());
      return T(vals[0], vals[1], vals[2]);
   }
   else if constexpr (esl::isOneOf_v<T, glm::vec4, glm::ivec4, glm::uvec4, glm::dvec4,
                                       glm::bvec4>)
   {
      using Value = typename T::value_type;
      using GlValue =
         typename std::conditional<std::is_same_v<Value, bool>, GLint, Value>::type;

      std::array<GlValue, 4> vals;
      getUniformValues(m_program, m_location, 4, vals.data());
      return T(vals[0], vals[1], vals[2], vals[3]);
   }
   else if (std::is_same_v<T, glm::mat2> || std::is_same_v<T, glm::dmat2>)
   {
      using GlValue = typename T::value_type;
      std::array<GlValue, 4> vals;
      getUniformValues(m_program, m_location, 4, vals.data());
      // getUniformValues() fills the data in column-major order. Pass values to the ctor
      // appropriately.
      return T(vals[0], vals[2], vals[1], vals[3]);
   }
   else if (std::is_same_v<T, glm::mat3> || std::is_same_v<T, glm::dmat3>)
   {
      using GlValue = typename T::value_type;
      std::array<GlValue, 9> vals;
      getUniformValues(m_program, m_location, 9, vals.data());
      // getUniformValues() fills the data in column-major order. Pass values to the ctor
      // appropriately.
      return T(vals[0], vals[3], vals[6], vals[1], vals[4], vals[7], vals[2], vals[5],
               vals[7]);
   }
   else if (std::is_same_v<T, glm::mat4> || std::is_same_v<T, glm::dmat4>)
   {
      using GlValue = typename T::value_type;
      std::array<GlValue, 16> vals;
      getUniformValues(m_program, m_location, 16, vals.data());
      // getUniformValues() fills the data in column-major order. Pass values to the ctor
      // appropriately.
      return T(vals[0], vals[4], vals[8], vals[12], vals[1], vals[5], vals[9], vals[13],
               vals[2], vals[6], vals[10], vals[14], vals[3], vals[7], vals[11],
               vals[15]);
   }
#endif
   else
   {
      static_assert(false, "Unexpected uniform value type.");
   }

#pragma warning(default : 4201)
}


template <typename T> void Uniform::setValue(const T& val)
{
   if (hasLocation())
      setUniformValue(m_location, val);
}

} // namespace gll
