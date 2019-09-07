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

#ifndef MAT3_H
#define MAT3_H

#include "geom/Vec3.h"

namespace geom {

  template<typename T>
  struct Mat3 {
    using value_type = std::enable_if_t<std::is_floating_point_v<T>,T>;

    // Construction & Assignment /////////////////////////////////////////////

    Mat3() noexcept = default;

    Mat3(const Mat3<T>&) noexcept = default;
    Mat3<T>& operator=(const Mat3<T>&) noexcept = default;

    Mat3(Mat3<T>&&) noexcept = default;
    Mat3<T>& operator=(Mat3<T>&&) noexcept = default;

    Mat3(const T& _m00, const T& _m11, const T& _m22)
      : m00{_m00}
      , m11{_m11}
      , m22{_m22}
    {
    }

    Mat3(const T& _m00, const T& _m01, const T& _m02,
         const T& _m10, const T& _m11, const T& _m12,
         const T& _m20, const T& _m21, const T& _m22)
      : m00{_m00}
      , m01{_m01}
      , m02{_m02}
      , m10{_m10}
      , m11{_m11}
      , m12{_m12}
      , m20{_m20}
      , m21{_m21}
      , m22{_m22}
    {
    }

    // Special Matrices //////////////////////////////////////////////////////

    static constexpr Mat3<T> identity()
    {
      return Mat3<T>{1, 1, 1};
    }

    static inline Mat3<T> rotateX(const T& angle)
    {
      const T COS = std::cos(angle);
      const T SIN = std::sin(angle);
      return Mat3<T>{
        1, 0, 0,
        0, COS, -SIN,
            0, SIN, COS
      };
    }

    static inline Mat3<T> rotateY(const T& angle)
    {
      const T COS = std::cos(angle);
      const T SIN = std::sin(angle);
      return Mat3<T>{
        COS, 0, SIN,
            0, 1, 0,
            -SIN, 0, COS
      };
    }

    static inline Mat3<T> rotateZ(const T& angle)
    {
      const T COS = std::cos(angle);
      const T SIN = std::sin(angle);
      return Mat3<T>{
        COS, -SIN, 0,
            SIN, COS, 0,
            0, 0, 1
      };
    }

    static constexpr Mat3<T> scale(const T& sx, const T& sy, const T& sz)
    {
      return Mat3<T>{sx, sy, sz};
    }

    // Operators /////////////////////////////////////////////////////////////

    constexpr Mat3<T> operator+() const
    {
      return Mat3<T>{m00, m01, m02, m10, m11, m12, m20, m21, m22};
    }

    constexpr Mat3<T> operator-() const
    {
      return Mat3<T>{-m00, -m01, -m02, -m10, -m11, -m12, -m20, -m21, -m22};
    }

    // Determinant ///////////////////////////////////////////////////////////

    constexpr T cofactor00() const
    {
      return m11*m22 - m21*m12;
    }

    constexpr T cofactor01() const
    {
      return m10*m22 - m20*m12;
    }

    constexpr T cofactor02() const
    {
      return m10*m21 - m20*m11;
    }

    constexpr T cofactor10() const
    {
      return m01*m22 - m21*m02;
    }

    constexpr T cofactor11() const
    {
      return m00*m22 - m20*m02;
    }

    constexpr T cofactor12() const
    {
      return m00*m21 - m20*m01;
    }

    constexpr T cofactor20() const
    {
      return m01*m12 - m11*m02;
    }

    constexpr T cofactor21() const
    {
      return m00*m12 - m10*m02;
    }

    constexpr T cofactor22() const
    {
      return m00*m11 - m10*m01;
    }

    constexpr T determinant() const
    {
      return m00*cofactor00() - m01*cofactor01() + m02*cofactor02();
    }

    // Inverse ///////////////////////////////////////////////////////////////

    inline Mat3<T> inverse(bool *ok = nullptr) const
    {
      if( ok != nullptr ) {
        *ok = false;
      }

      const T c00 = cofactor00();
      const T c01 = cofactor01();
      const T c02 = cofactor02();

      const T det{m00*c00 - m01*c01 + m02*c02};
      if( det == T{0} ) {
        return Mat3<T>{};
      }

      const T c10 = cofactor10();
      const T c11 = cofactor11();
      const T c12 = cofactor12();

      const T c20 = cofactor20();
      const T c21 = cofactor21();
      const T c22 = cofactor22();

      if( ok != nullptr ) {
        *ok = true;
      }

      return Mat3<T>{
        c00/det, -c10/det, c20/det,
            -c01/det, c11/det, -c21/det,
            c02/det, -c12/det, c22/det
      };
    }

    // Transpose /////////////////////////////////////////////////////////////

    inline Mat3<T>& transpose()
    {
      std::swap(m01, m10);
      std::swap(m02, m20);
      std::swap(m12, m21);
      return *this;
    }

    constexpr Mat3<T> transposed() const
    {
      return Mat3<T>{m00, m10, m20, m01, m11, m21, m02, m12, m22};
    }

    // Elements //////////////////////////////////////////////////////////////

    value_type m00{}, m01{}, m02{}, m10{}, m11{}, m12{}, m20{}, m21{}, m22{};
  };

  // Operators ///////////////////////////////////////////////////////////////

  template<typename T>
  constexpr Mat3<T> operator+(const Mat3<T>& A, const Mat3<T>& B)
  {
#define ADD(i,j)  A.m##i##j + B.m##i##j
    return Mat3<T>{
      ADD(0,0), ADD(0,1), ADD(0,2),
          ADD(1,0), ADD(1,1), ADD(1,2),
          ADD(2,0), ADD(2,1), ADD(2,2)
    };
#undef ADD
  }

  template<typename T>
  constexpr Mat3<T> operator-(const Mat3<T>& A, const Mat3<T>& B)
  {
#define SUB(i,j)  A.m##i##j - B.m##i##j
    return Mat3<T>{
      SUB(0,0), SUB(0,1), SUB(0,2),
          SUB(1,0), SUB(1,1), SUB(1,2),
          SUB(2,0), SUB(2,1), SUB(2,2)
    };
#undef SUB
  }

  template<typename T>
  constexpr Mat3<T> operator*(const T& s, const Mat3<T>& M)
  {
#define SMUL(i,j)  s*M.m##i##j
    return Mat3<T>{
      SMUL(0,0), SMUL(0,1), SMUL(0,2),
          SMUL(1,0), SMUL(1,1), SMUL(1,2),
          SMUL(2,0), SMUL(2,1), SMUL(2,2)
    };
#undef SMUL
  }

  template<typename T>
  constexpr Mat3<T> operator*(const Mat3<T>& M, const T& s)
  {
#define SMUL(i,j)  s*M.m##i##j
    return Mat3<T>{
      SMUL(0,0), SMUL(0,1), SMUL(0,2),
          SMUL(1,0), SMUL(1,1), SMUL(1,2),
          SMUL(2,0), SMUL(2,1), SMUL(2,2)
    };
#undef SMUL
  }

  template<typename T>
  constexpr Vec3<T> operator*(const Mat3<T>& M, const Vec3<T>& v)
  {
#define VPROD(i)  M.m##i##0*v.x + M.m##i##1*v.y + M.m##i##2*v.z
    return Vec3<T>{
      VPROD(0),
          VPROD(1),
          VPROD(2)
    };
#undef VPROD
  }

  template<typename T>
  constexpr Mat3<T> operator*(const Mat3<T>& A, const Mat3<T>& B)
  {
#define MPROD(i,j)  A.m##i##0*B.m0##j + A.m##i##1*B.m1##j + A.m##i##2*B.m2##j
    return Mat3<T>{
      MPROD(0,0), MPROD(0,1), MPROD(0,2),
          MPROD(1,0), MPROD(1,1), MPROD(1,2),
          MPROD(2,0), MPROD(2,1), MPROD(2,2)
    };
#undef MPROD
  }

  template<typename T>
  constexpr Mat3<T> operator/(const Mat3<T>& M, const T& s)
  {
#define SDIV(i,j)  M.m##i##j/s
    return Mat3<T>{
      SDIV(0,0), SDIV(0,1), SDIV(0,2),
          SDIV(1,0), SDIV(1,1), SDIV(1,2),
          SDIV(2,0), SDIV(2,1), SDIV(2,2)
    };
#undef SDIV
  }

} // namespace geom

#endif // MAT3_H
