/****************************************************************************
** Copyright (c) 2021, Carsten Schmidt. All rights reserved.
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

#ifndef BOUNDS_H
#define BOUNDS_H

#include "geom/Ray.h"

namespace geom {

  class Bounds {
  public:
    static constexpr real_t MAX_REAL_T = math::Max<real_t>;
    static constexpr real_t MIN_REAL_T = math::Min<real_t>;
    static constexpr real_t        ONE = math::ONE<real_t>;

    Bounds() noexcept
      : _min(MAX_REAL_T)
      , _max(MIN_REAL_T)
    {
    }

    Bounds(const Vertex& p1, const Vertex& p2) noexcept
    {
      set(p1, p2);
    }

    ~Bounds() noexcept = default;

    inline bool isValid() const
    {
      return simd::cmpLEQ<false>(_min.eval(), _max.eval());
    }

    void set(const Vertex& p1, const Vertex& p2)
    {
      _min = n4::min(p1, p2);
      _max = n4::max(p1, p2);
    }

    inline Vertex min() const
    {
      return _min;
    }

    inline Vertex max() const
    {
      return _max;
    }

    bool intersect(const Ray& ray) const
    {
      real_t t0 = 0;
      real_t t1 = ray.tMax();
      for(size_t i = 0; i < 3; i++) {
        const real_t invDir = ONE/ray.direction()(i);
        real_t tNear = (_min(i) - ray.origin()(i))*invDir;
        real_t tFar  = (_max(i) - ray.origin()(i))*invDir;
        if( tNear > tFar ) {
          std::swap<real_t>(tNear, tFar);
        }
        t0 = tNear > t0 ? tNear : t0;
        t1 = tFar  < t1 ? tFar  : t1;
        if( t0 > t1 ) {
          return false;
        }
      }

      return true;
    }

  private:
    Vertex _min;
    Vertex _max;
  };

} // namespace geom

#endif // BOUNDS_H
