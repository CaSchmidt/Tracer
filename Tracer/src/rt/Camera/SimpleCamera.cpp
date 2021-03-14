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

#include "rt/Camera/SimpleCamera.h"

namespace rt {

  ////// public //////////////////////////////////////////////////////////////

  SimpleCamera::SimpleCamera(const RenderOptions& options)
    : ICamera(options.width, options.height)
    , _options(options)
  {
    setup();
  }

  SimpleCamera::~SimpleCamera()
  {
  }

  Ray SimpleCamera::ray(const size_t x, const size_t y, const SamplerPtr& sampler) const
  {
    return makeRay(_windowTransform, x, y, sampler);
  }

  CameraPtr SimpleCamera::create(const RenderOptions& options)
  {
    return std::make_unique<SimpleCamera>(options);
  }

  ////// private /////////////////////////////////////////////////////////////

  bool SimpleCamera::setup()
  {
    if( !isValidFoV(_options.fov_rad) ) {
      return false;
    }

    _windowTransform = windowTransform(width(), height(), _options.fov_rad);

    return true;
  }

  /*
   * NOTE:
   * Cf. to Scratchapixel v2.0 for the derivation of the window transform:
   * https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-generating-camera-rays/generating-camera-rays
   */
  Matrix SimpleCamera::windowTransform(const size_t width, const size_t height, const real_t fov_rad)
  {
    const real_t w = static_cast<real_t>(width);
    const real_t h = static_cast<real_t>(height);
    const real_t a = w/h;
    const real_t n = n4::tan(fov_rad/TWO);
    return n4::translate(-a*n, n, -1)*n4::scale(TWO*a*n/w, -TWO*n/h, 1);
  }

} // namespace rt
