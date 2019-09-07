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

#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <cstdint>

#include <type_traits>

#include "geom/Util.h"

namespace geom {

  template<typename T>
  struct Vec3 {
    using value_type = std::enable_if_t<std::is_floating_point_v<T>,T>;

    // Construction & Assignment /////////////////////////////////////////////

    Vec3() noexcept = default;

    Vec3(const Vec3<T>&) noexcept = default;
    Vec3<T>& operator=(const Vec3<T>&) noexcept = default;

    Vec3(Vec3<T>&&) noexcept = default;
    Vec3<T>& operator=(Vec3<T>&&) noexcept = default;

    Vec3(const T& _x, const T& _y, const T& _z) noexcept
      : x{_x}
      , y{_y}
      , z{_z}
    {
    }

    // Special Vectors ///////////////////////////////////////////////////////

    static constexpr Vec3<T> zero()
    {
      return Vec3<T>{0, 0, 0};
    }

    static constexpr Vec3<T> xAxis()
    {
      return Vec3<T>{1, 0, 0};
    }

    static constexpr Vec3<T> yAxis()
    {
      return Vec3<T>{0, 1, 0};
    }

    static constexpr Vec3<T> zAxis()
    {
      return Vec3<T>{0, 0, 1};
    }

    // Operators /////////////////////////////////////////////////////////////

    constexpr Vec3<T> operator+() const
    {
      return Vec3<T>{x, y, z};
    }

    constexpr Vec3<T> operator-() const
    {
      return Vec3<T>{-x, -y, -z};
    }

    inline Vec3<T>& operator+=(const Vec3<T>& v)
    {
      x += v.x;
      y += v.y;
      z += v.z;
      return *this;
    }

    inline Vec3<T>& operator-=(const Vec3<T>& v)
    {
      x -= v.x;
      y -= v.y;
      z -= v.z;
      return *this;
    }

    inline Vec3<T>& operator*=(const T& s)
    {
      x *= s;
      y *= s;
      z *= s;
      return *this;
    }

    inline Vec3<T>& operator/=(const T& s)
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

    inline Vec3<T>& normalize()
    {
      const T len = length();
      x /= len;
      y /= len;
      z /= len;
      return *this;
    }

    constexpr Vec3<T> normalized() const
    {
      return Vec3<T>{x/length(), y/length(), z/length()};
    }

    // Conversion ////////////////////////////////////////////////////////////

    constexpr Vec3<T> clamped(const T& lo, const T& hi) const
    {
      return Vec3<T>{clamp<T>(x, lo, hi), clamp<T>(y, lo, hi), clamp<T>(z, lo, hi)};
    }

    constexpr uint8_t r() const
    {
      return static_cast<uint8_t>(clamp<T>(x, 0, 1)*T{255});
    }

    constexpr uint8_t g() const
    {
      return static_cast<uint8_t>(clamp<T>(y, 0, 1)*T{255});
    }

    constexpr uint8_t b() const
    {
      return static_cast<uint8_t>(clamp<T>(z, 0, 1)*T{255});
    }

    // Elements //////////////////////////////////////////////////////////////

    value_type x{}, y{}, z{};
  };

} // namespace geom

namespace geom {

  // Operators ///////////////////////////////////////////////////////////////

  template<typename T>
  constexpr Vec3<T> operator+(const Vec3<T>& a, const Vec3<T>& b)
  {
    return Vec3<T>{a.x + b.x, a.y + b.y, a.z + b.z};
  }

  template<typename T>
  constexpr Vec3<T> operator-(const Vec3<T>& a, const Vec3<T>& b)
  {
    return Vec3<T>{a.x - b.x, a.y - b.y, a.z - b.z};
  }

  template<typename T>
  constexpr Vec3<T> operator*(const T& s, const Vec3<T>& v)
  {
    return Vec3<T>{v.x*s, v.y*s, v.z*s};
  }

  template<typename T>
  constexpr Vec3<T> operator*(const Vec3<T>& v, const T& s)
  {
    return Vec3<T>{v.x*s, v.y*s, v.z*s};
  }

  template<typename T>
  constexpr Vec3<T> operator/(const Vec3<T>& v, const T& s)
  {
    return Vec3<T>{v.x/s, v.y/s, v.z/s};
  }

  // Functions ///////////////////////////////////////////////////////////////

  template<typename T>
  constexpr Vec3<T> cross(const Vec3<T>& a, const Vec3<T>& b)
  {
    return Vec3<T>{
      a.y*b.z - a.z*b.y,
          a.z*b.x - a.x*b.z,
          a.x*b.y - a.y*b.x
    };
  }

  template<typename T>
  constexpr Vec3<T> direction(const Vec3<T>& from, const Vec3<T>& to)
  {
    return (to - from).normalized();
  }

  template<typename T>
  constexpr T distance(const Vec3<T>& from, const Vec3<T>& to)
  {
    return (to - from).length();
  }

  template<typename T>
  constexpr T dot(const Vec3<T>& a, const Vec3<T>& b)
  {
    return a.x*b.x + a.y*b.y + a.z*b.z;
  }

  template<typename T>
  constexpr T dot01(const Vec3<T>& a, const Vec3<T>& b)
  {
    return std::max<T>(0, dot(a,b));
  }

  template<typename T>
  constexpr Vec3<T> mulCWise(const Vec3<T>& a, const Vec3<T>& b)
  {
    return Vec3<T>{a.x*b.x, a.y*b.y, a.z*b.z};
  }

} // namespace geom

#endif // VEC3_H
