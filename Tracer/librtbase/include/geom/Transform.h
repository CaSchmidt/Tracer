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

#include <N4/Util.h>

#include "geom/Bounds.h"

namespace geom {

  class Transform {
  public:
    // Construction & Assignment /////////////////////////////////////////////

    Transform(const Transform&) noexcept = default;
    Transform& operator=(const Transform&) = default;

    Transform(Transform&&) noexcept = default;
    Transform& operator=(Transform&&) = default;

    ~Transform() noexcept = default;

    Transform(const Matrix& X = n4::identity()) noexcept
      : _X{X}
    {
      _Xinv = _X.inverse();
      _Xn   = _Xinv.transpose();
    }

    // Operators /////////////////////////////////////////////////////////////

    inline Direction operator*(const Direction& d) const
    {
      return _X*d;
    }

    inline Normal operator*(const Normal& n) const
    {
      return _Xn*n;
    }

    inline Vertex operator*(const Vertex& v) const
    {
      return _X*v;
    }

    inline Bounds operator*(const Bounds& bounds) const
    {
      const Vertex p1 = _X*bounds.min();
      const Vertex p2 = _X*bounds.max();
      return Bounds(p1, p2);
    }

    inline Ray operator*(const Ray& ray) const
    {
      return Ray(operator*(ray.origin()), operator*(ray.direction()), ray.tMax());
    }

    inline Transform operator*(const Transform& other) const
    {
      return Transform(_X*other._X);
    }

    // Special Transforms ////////////////////////////////////////////////////

    static inline Transform lookAt(const Vertex& from, const Vertex& to,
                                   const Direction& up)
    {
      return Transform(n4::util::lookAt(from, to, up));
    }

    static inline Transform rotateZYX(const real_t rz, const real_t ry, const real_t rx)
    {
      return Transform(
            n4::rotateZ(rz)*n4::rotateY(ry)*n4::rotateX(rx)
            );
    }

    static inline Transform rotateZYXbyPI2(const signed int iz, const signed int iy, const signed int ix)
    {
      return Transform(
            n4::rotateZbyPI2(iz)*n4::rotateYbyPI2(iy)*n4::rotateXbyPI2(ix)
            );
    }

    static inline Transform translate(const real_t tx, const real_t ty, const real_t tz)
    {
      return Transform(n4::translate(tx, ty, tz));
    }

    // Inverse ///////////////////////////////////////////////////////////////

    inline Transform inverse() const
    {
      return Transform(_Xinv);
    }

  private:
    Matrix _X{};
    Matrix _Xinv{};
    Matrix _Xn{};
  };

} // namespace geom

#endif // TRANSFORM_H
