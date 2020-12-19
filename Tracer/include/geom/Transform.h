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

#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "geom/Ray.h"

namespace geom {

  template<typename T>
  class Transform {
  public:
    // Construction & Assignment /////////////////////////////////////////////

    Transform(const Transform<T>&) noexcept = default;
    Transform<T>& operator=(const Transform<T>&) noexcept = default;

    Transform(Transform<T>&&) noexcept = default;
    Transform<T>& operator=(Transform<T>&&) noexcept = default;

    ~Transform() noexcept = default;

    Transform(const Matrix<T>& X = geom::identity<T>(), const Vertex<T>& t = Vertex<T>{}) noexcept
      : _t{t}
      , _X{X}
    {
      _haveInv = cs::determinant(X) != 0;
      if( _haveInv ) {
        _Xinv = cs::inverse(X);
      } else {
        _Xinv = 0;
      }
      _Xn = cs::transpose(_Xinv);
    }

    // Operators /////////////////////////////////////////////////////////////

    inline Normal<T> operator*(const Normal<T>& n) const
    {
      return _Xn*n;
    }

    inline Vertex<T> operator*(const Vertex<T>& v) const
    {
      return _X*v + _t;
    }

    inline Transform<T> operator*(const Transform<T>& other) const
    {
      return Transform<T>{_X*other._X, _t + _X*other._t};
    }

    inline Ray<T> operator*(const Ray<T>& ray) const
    {
      return Ray<T>{_X*ray.origin() + _t, _X*ray.direction()};
    }

    // Special Transforms ////////////////////////////////////////////////////

    /*
     * NOTE:
     * lookAt() uses OpenGL's camera conventions (AKA is right-handed):
     * - the viewer looks along the negative z axis
     * - x points right
     * - y points up
     */
    static inline Transform<T> lookAt(const Vertex<T>& from, const Vertex<T>& to,
                                      const Normal<T>& up)
    {
      const Normal<T> z = to_normal<T>(cs::direction(to, from)); // Looking along the NEGATIVE z axis!
      const Normal<T> x = cs::normalize(cs::cross(up, z));
      const Normal<T> y = cs::normalize(cs::cross(z, x));
      return Transform<T>{{ x.x, y.x, z.x, x.y, y.y, z.y, x.z, y.z, z.z }, from};
    }

    static inline Transform<T> rotateZYX(const T rz, const T ry, const T rx)
    {
      return Transform<T>{
        geom::rotateZ<T>(rz)*geom::rotateY<T>(ry)*geom::rotateX<T>(rx)
      };
    }

    static inline Transform<T> rotateZYXbyPI2(const signed int iz, const signed int iy, const signed int ix)
    {
      return Transform<T>{
        geom::rotateZbyPI2<T>(iz)*geom::rotateYbyPI2<T>(iy)*geom::rotateXbyPI2<T>(ix)
      };
    }

    static inline Transform<T> scale(const T sx, const T sy, const T sz)
    {
      return Transform<T>{geom::scale<T>(sx, sy, sz)};
    }

    static inline Transform<T> translate(const Vertex<T>& t)
    {
      return Transform<T>{geom::identity<T>(), t};
    }

    // Inverse ///////////////////////////////////////////////////////////////

    inline Transform<T> inverse(bool *ok = nullptr) const
    {
      if( ok != nullptr ) {
        *ok = _haveInv;
      }
      return Transform<T>{_Xinv, -_Xinv*_t};
    }

    // Normal Transform //////////////////////////////////////////////////////

    inline const Matrix<T>& normalTransform(bool *ok = nullptr) const
    {
      if( ok != nullptr ) {
        *ok = _haveInv;
      }
      return _Xn;
    }

    // Element Access ////////////////////////////////////////////////////////

    inline const Matrix<T>& scaledRotation() const
    {
      return _X;
    }

    inline const Vertex<T>& translation() const
    {
      return _t;
    }

  private:
    bool _haveInv{};
    Vertex<T> _t{};
    Matrix<T> _X{}, _Xinv{}, _Xn{};
  };

} // namespace geom

#endif // TRANSFORM_H
