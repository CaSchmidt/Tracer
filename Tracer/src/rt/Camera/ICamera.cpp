/****************************************************************************
** Copyright (c) 2020, Carsten Schmidt. All rights reserved.
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

#include "rt/Camera/ICamera.h"

#include "rt/Sampler/SimpleSampler.h"

namespace rt {

  ////// public //////////////////////////////////////////////////////////////

  ICamera::ICamera()
  {
    _sampler = SimpleSampler::create();
  }

  ICamera::~ICamera()
  {
  }

  ////// protected ///////////////////////////////////////////////////////////

  Image ICamera::create_image(const size_t width, const size_t height,
                              size_t& y0, size_t& y1) const
  {
    if( width < 1  ||  height < 1 ) {
      return Image();
    }
    y0 = std::clamp<size_t>(y0, 0, height);
    y1 = std::clamp<size_t>(y1, 0, height);
    if( y0 == y1  ||  y0 >= height ) {
      return Image();
    }
    return Image(width, y1 - y0);
  }

  bool ICamera::isValidFoV(const real_t fov_rad)
  {
    return ZERO < fov_rad  &&  fov_rad < PI;
  }

  Ray ICamera::ray(const Matrix& W, const size_t x, const size_t y,
                   const bool random) const
  {
    const real_t dx = random
        ? _sampler->sample()
        : ONE_HALF;
    const real_t dy = random
        ? _sampler->sample()
        : ONE_HALF;

    const Vertex org = W*Vertex{static_cast<real_t>(x) + dx, static_cast<real_t>(y) + dy};

    return Ray(org, geom::to_direction(org));
  }

} // namespace rt
