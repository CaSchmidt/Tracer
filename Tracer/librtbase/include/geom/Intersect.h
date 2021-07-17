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

    namespace priv {

      inline real_t quadraticHit(const real_t a, const real_t b, const real_t c,
                                 const real_t epsilon0)
      {
        real_t t1, t2;
        if( !math::quadratic<real_t>(a, b, c, &t1, &t2) ) {
          return NO_INTERSECTION;
        }
        // NOTE: Assumes t1 < t2 !!!
        if( t1 >= epsilon0 ) {
          return t1;
        }
        if( t2 >= epsilon0 ) {
          return t2;
        }
        return NO_INTERSECTION;
      }

    } // namespace priv

    // Constants /////////////////////////////////////////////////////////////

    inline constexpr real_t TWO = 2;

    // Intersection Tests ////////////////////////////////////////////////////

    inline real_t cylinder(const Ray& ray, const real_t r, const real_t epsilon0)
    {
      const real_t ox = ray.origin().x;
      const real_t oy = ray.origin().y;
      const real_t dx = ray.direction().x;
      const real_t dy = ray.direction().y;
      const real_t  a =      dx*dx + dy*dy;
      const real_t  b = TWO*(dx*ox + dy*oy);
      const real_t  c =      ox*ox + oy*oy - r*r;

      return priv::quadraticHit(a, b, c, epsilon0);
    }

    inline real_t plane(const Ray& ray, const real_t epsilon0, const real_t h = ZERO)
    {
      if( ray.direction().z == ZERO ) {
        return NO_INTERSECTION;
      }
      const real_t t0 = (h - ray.origin().z)/ray.direction().z;

      return t0 >= epsilon0
          ? t0
          : NO_INTERSECTION;
    }

    inline real_t sphere(const Ray& ray, const real_t r, const real_t epsilon0)
    {
      const real_t a =     n4::dot(ray.direction(), ray.direction());
      const real_t b = TWO*n4::dot(ray.direction(), to_direction(ray.origin()));
      const real_t c =     n4::dot(ray.origin(),    ray.origin()) - r*r;

      return priv::quadraticHit(a, b, c, epsilon0);
    }

  } // namespace intersect

} // namespace geom

#endif // INTERSECT_H
