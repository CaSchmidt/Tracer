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

#include "rt/Camera.h"

namespace rt {

  namespace priv {

    constexpr real_T scaledPos(const real_T& in, const real_T& s1, const real_T& s2,
                               const real_T& delta = ONE_HALF)
    {
      return s2*(TWO*(in + delta)/s1 - ONE);
    }

  } // namespace priv

  ////// public //////////////////////////////////////////////////////////////

  Camera::Camera(const Vertex3f& eye, const Vertex3f& lookAt, const Normal3f& up,
                 const std::size_t width, const std::size_t height, const real_T fov_rad) noexcept
    : _aspect{static_cast<real_T>(width)/height}
    , _width{width}
    , _height{height}
    , _eye{eye}
  {
    rand_init();

    _cam = Transformf::lookAt(eye, lookAt, up);

    _near = -_aspect/csTan(fov_rad/TWO);
  }

  Rayf Camera::ray(const std::size_t _x, const std::size_t _y, const bool random) const
  {
    if( _x >= _width  ||  _y >= _height ) {
      return Rayf();
    }

    const real_T dx = random
        ? rand()
        : ONE_HALF;
    const real_T dy = random
        ? rand()
        : ONE_HALF;

    const real_T x = priv::scaledPos(real_T(_x), real_T(_width), _aspect, dx);
    const real_T y = priv::scaledPos(real_T(_y), real_T(_height), -ONE, dy);

    const Vertex3f org{x, y, _near};

    return (_cam*Rayf{org, geom::to_normal<real_T>(org)});
  }

  ////// private /////////////////////////////////////////////////////////////

  void Camera::rand_init()
  {
    _randDis = std::uniform_real_distribution<real_T>(ZERO, ONE);

    std::random_device randDev;
    _randGen.seed(randDev());
  }

  real_T Camera::rand() const
  {
    return _randDis(*const_cast<std::mt19937*>(&_randGen));
  }

} // namespace rt
