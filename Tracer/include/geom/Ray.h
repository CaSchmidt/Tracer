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

#include "geom/Vector.h"

namespace geom {

  template<typename T>
  class Ray {
  public:
    Ray() noexcept = default;

    Ray(const Ray&) noexcept = default;
    Ray& operator=(const Ray&) noexcept = default;

    Ray(Ray&&) noexcept = default;
    Ray& operator=(Ray&&) noexcept = default;

    Ray(const Vertex<T>& org, const Normal<T>& dir) noexcept
      : _org{org}
      , _dir{dir.normalized()}
    {
    }

    ~Ray() noexcept = default;

    constexpr Vertex<T> operator()(const T& t) const
    {
      return _org + t*_dir;
    }

    constexpr Vertex<T> origin() const
    {
      return _org;
    }

    constexpr Normal<T> direction() const
    {
      return _dir;
    }

  private:
    Vertex<T> _org{};
    Normal<T> _dir{};
  };

} // namespace geom

#endif // RAY_H
