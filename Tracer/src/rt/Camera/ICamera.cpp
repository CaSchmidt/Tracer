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

namespace rt {

  ////// public //////////////////////////////////////////////////////////////

  ICamera::ICamera()
  {
    rand_init();
  }

  ICamera::~ICamera()
  {
  }

  ////// protected ///////////////////////////////////////////////////////////

  Image ICamera::create_image(const std::size_t width, const std::size_t height,
                              std::size_t& y0, std::size_t& y1) const
  {
    if( width < 1  ||  height < 1 ) {
      return Image();
    }
    y0 = std::clamp<std::size_t>(y0, 0, height);
    y1 = std::clamp<std::size_t>(y1, 0, height);
    if( y0 == y1  ||  y0 >= height ) {
      return Image();
    }
    return Image(width, y1 - y0);
  }

  bool ICamera::isValidFoV(const real_T fov_rad)
  {
    return ZERO < fov_rad  &&  fov_rad < PI;
  }

  real_T ICamera::rand() const
  {
    return _randDis(*const_cast<std::mt19937*>(&_randGen));
  }

  Rayf ICamera::ray(const Matrix3f& W, const std::size_t x, const std::size_t y,
                    const bool random) const
  {
    const real_T dx = random
        ? rand()
        : ONE_HALF;
    const real_T dy = random
        ? rand()
        : ONE_HALF;

    const Vertex3f org = W*Vertex3f{static_cast<real_T>(x) + dx, static_cast<real_T>(y) + dy, 1};

    return Rayf{org, to_normal(org)};
  }

  ////// private /////////////////////////////////////////////////////////////

  void ICamera::rand_init()
  {
    _randDis = std::uniform_real_distribution<real_T>(ZERO, ONE);

    std::random_device randDev;
    _randGen.seed(randDev());
  }

} // namespace rt
