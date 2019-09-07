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

#ifndef INTERSECT_H
#define INTERSECT_H

#include <limits>

#include "geom/Ray.h"
#include "math/Quadratic.h"

namespace geom {

  template<typename T>
  inline T intersectCylinder(const Ray<T>& ray,
                             const Vec3<T>& C, // cylinder's origin
                             const Vec3<T>& N, // cylinder's normalized z-axis
                             const T& r)       // cylinder's radius
  {
    constexpr T NO_INTERSECTION = std::numeric_limits<T>::quiet_NaN();

    const Vec3<T> C_ = C               - dot(C,               N)*N;
    const Vec3<T> D_ = ray.direction() - dot(ray.direction(), N)*N;
    const Vec3<T> O_ = ray.origin()    - dot(ray.origin(),    N)*N;

    const Vec3<T> D = O_ - C_;

    const T a = dot(D_, D_);
    const T b = static_cast<T>(2)*dot(D_, D);
    const T c = dot(D, D) - r*r;

    T t1, t2;
    if( !math::solveQuadratic(a, b, c, t1, t2) ) {
      return NO_INTERSECTION;
    }

    if( t1 < 0 ) {
      t1 = t2;
      if( t1 < 0 ) {
        return NO_INTERSECTION;
      }
    }

    return t1;
  }

  template<typename T>
  inline T intersectPlane(const Ray<T>& ray,
                          const Vec3<T>& P, // plane's origin
                          const Vec3<T>& N) // plane's normal
  {
    constexpr T NO_INTERSECTION = std::numeric_limits<T>::quiet_NaN();

    const T den = dot(N, ray.direction());
    if( den == 0.0 ) {
      return NO_INTERSECTION;
    }

    const T tHit = dot(N, P - ray.origin())/den;
    return tHit >= 0
        ? tHit
        : NO_INTERSECTION;
  }

  template<typename T>
  inline T intersectSphere(const Ray<T>& ray,
                           const Vec3<T>& S, // sphere's origin
                           const T& r)       // sphere's radius
  {
    constexpr T NO_INTERSECTION = std::numeric_limits<T>::quiet_NaN();

    const Vec3<T> D{ray.origin() - S};

    const T a = 1;
    const T b = static_cast<T>(2)*dot(ray.direction(), D);
    const T c = dot(D, D) - r*r;

    T t1, t2;
    if( !math::solveQuadratic(a, b, c, t1, t2) ) {
      return NO_INTERSECTION;
    }

    if( t1 < 0 ) { // ray is at least inside sphere
      t1 = t2;
      if( t1 < 0 ) { // sphere is behind ray
        return NO_INTERSECTION;
      }
    }

    return t1; // return the nearest hit from the ray's origin
  }

} // namespace geom

#endif // INTERSECT_H
