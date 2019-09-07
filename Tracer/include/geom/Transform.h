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

#include "geom/Mat3.h"
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

    Transform(const Mat3<T>& X = Mat3<T>::identity(), const Vec3<T>& t = Vec3<T>{}) noexcept
      : _t{t}
      , _X{X}
    {
      _Xinv = _X.inverse(&_haveInv);
    }

    // Special Transforms ////////////////////////////////////////////////////

    static constexpr Transform<T> rotateX(const T& angle)
    {
      return Transform<T>{Mat3<T>::rotateX(angle)};
    }

    static constexpr Transform<T> rotateY(const T& angle)
    {
      return Transform<T>{Mat3<T>::rotateY(angle)};
    }

    static constexpr Transform<T> rotateZ(const T& angle)
    {
      return Transform<T>{Mat3<T>::rotateZ(angle)};
    }

    static constexpr Transform<T> scale(const T& sx, const T& sy, const T& sz)
    {
      return Transform<T>{Mat3<T>::scale(sx, sy, sz)};
    }

    static constexpr Transform<T> translate(const Vec3<T>& t)
    {
      return Transform<T>{Mat3<T>::identity(), t};
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

    inline Mat3<T> normalTransform(bool *ok = nullptr) const
    {
      if( ok != nullptr ) {
        *ok = _haveInv;
      }
      return _Xinv.transposed();
    }

    // Element Access ////////////////////////////////////////////////////////

    constexpr const Mat3<T>& scaledRotation() const
    {
      return _X;
    }

    constexpr const Vec3<T>& translation() const
    {
      return _t;
    }

  private:
    bool _haveInv{};
    Vec3<T> _t{};
    Mat3<T> _X{}, _Xinv{};

    template<typename U>
    friend constexpr Vec3<U> operator*(const Transform<U>&, const Vec3<U>&);

    template<typename U>
    friend constexpr Transform<U> operator*(const Transform<U>&, const Transform<U>&);

    template<typename U>
    friend constexpr Ray<U> operator*(const Transform<U>&, const Ray<U>&);
  };

  // Operators ///////////////////////////////////////////////////////////////

  template<typename T>
  constexpr Vec3<T> operator*(const Transform<T>& M, const Vec3<T>& v)
  {
    return M._X*v + M._t;
  }

  template<typename T>
  constexpr Transform<T> operator*(const Transform<T>& M2, const Transform<T>& M1)
  {
    return Transform<T>{M2._X*M1._X, M2._t + M2._X*M1._t};
  }

  template<typename T>
  constexpr Ray<T> operator*(const Transform<T>& M, const Ray<T>& ray)
  {
    return Ray<T>{M*ray.origin(), M.scaledRotation()*ray.direction()};
  }

} // namespace geom

#endif // TRANSFORM_H
