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

#ifndef RAY_H
#define RAY_H

#include "geom/Geom.h"
#include "geom/Hit.h"

namespace geom {

  class Ray {
  public:
    static constexpr real_t MAX_T = math::Max<real_t>;
    static constexpr real_t  ZERO = intersect::ZERO;

    Ray() noexcept = default;

    Ray(const Ray&) noexcept = default;
    Ray& operator=(const Ray&) = default;

    Ray(Ray&&) noexcept = default;
    Ray& operator=(Ray&&) = default;

    Ray(const Vertex& org, const Direction& dir, const real_t tMax = MAX_T) noexcept
    {
      operator=(org);
      operator=(dir);
      setTMax(tMax);
    }

    ~Ray() noexcept = default;

    Ray& operator=(const Vertex& org)
    {
      _org = org;
      return *this;
    }

    Ray& operator=(const Direction& dir)
    {
      _dir = n4::normalize(dir);
      return *this;
    }

    inline Vertex operator()(const real_t t) const
    {
      return _org + to_vertex(t*_dir);
    }

    inline Vertex origin() const
    {
      return _org;
    }

    inline Direction direction() const
    {
      return _dir;
    }

    inline real_t tMax() const
    {
      return _tMax;
    }

    void setTMax(const real_t t)
    {
      _tMax = intersect::isHit(t)
          ? t
          : 0;
    }

    inline bool isValid() const
    {
      return Math::isFinite(_tMax)  &&  _tMax > ZERO;
    }

    inline bool isValidT(const real_t t) const
    {
      return geom::intersect::isHit(t)  &&  t < _tMax;
    }

  private:
    Vertex    _org{};
    Direction _dir{};
    real_t    _tMax{MAX_T};
  };

} // namespace geom

#endif // RAY_H
