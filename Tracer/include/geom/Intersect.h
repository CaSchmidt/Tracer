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

#include "geom/Ray.h"
#include "math/Solver.h"

namespace geom {

  namespace intersect {

    // Constants /////////////////////////////////////////////////////////////

    template<typename T>
    inline constexpr T NO_INTERSECTION = math::qNaN<T>;

    template<typename T> inline constexpr T ZERO = math::ZERO<T>;
    template<typename T> inline constexpr T  TWO = math::TWO<T>;

    // Intersection Tests ////////////////////////////////////////////////////

    template<typename T>
    inline T cylinder(const Ray<T>& ray, const T r)
    {
      const T ox = ray.origin().x;
      const T oy = ray.origin().y;
      const T dx = ray.direction().x;
      const T dy = ray.direction().y;
      const T  a =         dx*dx + dy*dy;
      const T  b = TWO<T>*(dx*ox + dy*oy);
      const T  c =         ox*ox + oy*oy - r*r;
      T t1, t2;
      if( !math::quadratic(a, b, c, t1, t2) ) {
        return NO_INTERSECTION<T>;
      }
      if( t1 < ZERO<T> ) {
        t1 = t2;
        if( t1 < ZERO<T> ) {
          return NO_INTERSECTION<T>;
        }
      }
      return t1;
    }

    template<typename T>
    inline T plane(const Ray<T>& ray, const T h = ZERO<T>)
    {
      if( ray.direction().z == ZERO<T> ) {
        return NO_INTERSECTION<T>;
      }
      const T t0 = (h - ray.origin().z)/ray.direction().z;
      return t0 >= ZERO<T>
          ? t0
          : NO_INTERSECTION<T>;
    }

    template<typename T>
    inline T sphere(const Ray<T>& ray, const T r)
    {
      const T a =        cs::dot(ray.direction(), ray.direction());
      const T b = TWO<T>*cs::dot(ray.direction(), to_normal<T>(ray.origin()));
      const T c =        cs::dot(ray.origin(),    ray.origin()) - r*r;
      T t1, t2;
      if( !math::quadratic(a, b, c, t1, t2) ) {
        return NO_INTERSECTION<T>;
      }
      if( t1 < ZERO<T> ) { // ray is at least inside sphere
        t1 = t2;
        if( t1 < ZERO<T> ) { // sphere is behind ray
          return NO_INTERSECTION<T>;
        }
      }
      return t1; // return nearest hit from the ray's origin
    }

  } // namespace intersect

} // namespace geom

#endif // INTERSECT_H
