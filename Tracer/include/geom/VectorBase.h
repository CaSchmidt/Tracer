/****************************************************************************
** Copyright (c) 2019, Carsten Schmidt. All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions
** are met:
**
** 1. Redistributions of source code must retain the above copyright
**    notice, this list of conditions and the following disclaimer.
**
** 2. Redistributions in binary form must reproduce the above copyright
**    notice, this list of conditions and the following disclaimer in the
**    documentation and/or other materials provided with the distribution.
**
** 3. Neither the name of the copyright holder nor the names of its
**    contributors may be used to endorse or promote products derived from
**    this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************/

#ifndef VECTORBASE_H
#define VECTORBASE_H

#include <cmath>
#include <cstdint>

#include <type_traits>

#include "geom/Util.h"

namespace geom {

  template<typename T, typename DerivedT>
  struct VectorBase {
    using value_type = std::enable_if_t<std::is_floating_point_v<T>,T>;

    // Construction & Assignment /////////////////////////////////////////////

    VectorBase() noexcept = default;

    VectorBase(const VectorBase<T,DerivedT>&) noexcept = default;
    VectorBase<T,DerivedT>& operator=(const VectorBase<T,DerivedT>&) noexcept = default;

    VectorBase(VectorBase<T,DerivedT>&&) noexcept = default;
    VectorBase<T,DerivedT>& operator=(VectorBase<T,DerivedT>&&) noexcept = default;

    VectorBase(const T& _x, const T& _y, const T& _z) noexcept
      : x{_x}
      , y{_y}
      , z{_z}
    {
    }

    // Operators /////////////////////////////////////////////////////////////

    constexpr VectorBase<T,DerivedT> operator+() const
    {
      return VectorBase<T,DerivedT>{x, y, z};
    }

    constexpr VectorBase<T,DerivedT> operator-() const
    {
      return VectorBase<T,DerivedT>{-x, -y, -z};
    }

    inline VectorBase<T,DerivedT>& operator+=(const VectorBase<T,DerivedT>& v)
    {
      x += v.x;
      y += v.y;
      z += v.z;
      return *this;
    }

    inline VectorBase<T,DerivedT>& operator-=(const VectorBase<T,DerivedT>& v)
    {
      x -= v.x;
      y -= v.y;
      z -= v.z;
      return *this;
    }

    inline VectorBase<T,DerivedT>& operator*=(const T& s)
    {
      x *= s;
      y *= s;
      z *= s;
      return *this;
    }

    inline VectorBase<T,DerivedT>& operator/=(const T& s)
    {
      x /= s;
      y /= s;
      z /= s;
      return *this;
    }

    // Length ////////////////////////////////////////////////////////////////

    constexpr T length() const
    {
      return std::sqrt(x*x + y*y + z*z);
    }

    inline VectorBase<T,DerivedT>& normalize()
    {
      const T len = length();
      x /= len;
      y /= len;
      z /= len;
      return *this;
    }

    constexpr VectorBase<T,DerivedT> normalized() const
    {
      return VectorBase<T,DerivedT>{x/length(), y/length(), z/length()};
    }

    // Conversion ////////////////////////////////////////////////////////////

    template<typename Type>
    constexpr VectorBase<T,Type> cast_to() const
    {
      return VectorBase<T,Type>{x, y, z};
    }

    constexpr VectorBase<T,DerivedT> clamped(const T& lo, const T& hi) const
    {
      return VectorBase<T,DerivedT>{clamp<T>(x, lo, hi), clamp<T>(y, lo, hi), clamp<T>(z, lo, hi)};
    }

    // Elements //////////////////////////////////////////////////////////////

    value_type x{}, y{}, z{};
  };

} // namespace geom

namespace geom {

  // Operators ///////////////////////////////////////////////////////////////

  template<typename T, typename TypeA, typename TypeB>
  constexpr VectorBase<T,TypeA> operator+(const VectorBase<T,TypeA>& a, const VectorBase<T,TypeB>& b)
  {
    return VectorBase<T,TypeA>{a.x + b.x, a.y + b.y, a.z + b.z};
  }

  template<typename T, typename TypeA, typename TypeB>
  constexpr VectorBase<T,TypeA> operator-(const VectorBase<T,TypeA>& a, const VectorBase<T,TypeB>& b)
  {
    return VectorBase<T,TypeA>{a.x - b.x, a.y - b.y, a.z - b.z};
  }

  template<typename T, typename DerivedT>
  constexpr VectorBase<T,DerivedT> operator*(const T& s, const VectorBase<T,DerivedT>& v)
  {
    return VectorBase<T,DerivedT>{v.x*s, v.y*s, v.z*s};
  }

  template<typename T, typename DerivedT>
  constexpr VectorBase<T,DerivedT> operator*(const VectorBase<T,DerivedT>& v, const T& s)
  {
    return VectorBase<T,DerivedT>{v.x*s, v.y*s, v.z*s};
  }

  template<typename T, typename DerivedT>
  constexpr VectorBase<T,DerivedT> operator/(const VectorBase<T,DerivedT>& v, const T& s)
  {
    return VectorBase<T,DerivedT>{v.x/s, v.y/s, v.z/s};
  }

  // Functions ///////////////////////////////////////////////////////////////

  template<typename T, typename TypeA, typename TypeB>
  constexpr VectorBase<T,TypeA> cross(const VectorBase<T,TypeA>& a, const VectorBase<T,TypeB>& b)
  {
    return VectorBase<T,TypeA>{
      a.y*b.z - a.z*b.y,
          a.z*b.x - a.x*b.z,
          a.x*b.y - a.y*b.x
    };
  }

  template<typename T, typename TypeA, typename TypeB>
  constexpr VectorBase<T,TypeA> direction(const VectorBase<T,TypeA>& from, const VectorBase<T,TypeB>& to)
  {
    return (to - from).normalized();
  }

  template<typename T, typename TypeA, typename TypeB>
  constexpr T distance(const VectorBase<T,TypeA>& from, const VectorBase<T,TypeB>& to)
  {
    return (to - from).length();
  }

  template<typename T, typename TypeA, typename TypeB>
  constexpr T dot(const VectorBase<T,TypeA>& a, const VectorBase<T,TypeB>& b)
  {
    return a.x*b.x + a.y*b.y + a.z*b.z;
  }

  template<typename T, typename TypeA, typename TypeB>
  constexpr T dot01(const VectorBase<T,TypeA>& a, const VectorBase<T,TypeB>& b)
  {
    return std::max<T>(0, dot(a, b));
  }

  template<typename T, typename TypeA, typename TypeB>
  constexpr VectorBase<T,TypeA> mulCWise(const VectorBase<T,TypeA>& a, const VectorBase<T,TypeB>& b)
  {
    return VectorBase<T,TypeA>{a.x*b.x, a.y*b.y, a.z*b.z};
  }

} // namespace geom

#endif // VECTORBASE_H
