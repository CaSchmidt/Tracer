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

#include "rt/Camera/FrustumCamera.h"

#include "rt/Renderer/RenderOptions.h"
#include "rt/Sampler/Sampling.h"

namespace rt {

  ////// public //////////////////////////////////////////////////////////////

  FrustumCamera::FrustumCamera(const size_t width, const size_t height,
                               const real_t fov_rad, const real_t worldToScreen,
                               const real_t aperture, const real_t focus)
    : ICamera(width, height)
  {
    if( !isValidFoV(fov_rad)  ||  worldToScreen <= ZERO ) {
      return;
    }

    if( aperture > ZERO  &&  focus > ZERO ) {
      _rLens  = aperture/TWO;
      _zFocus = -focus;
    } else {
      _rLens = _zFocus = 0;
    }

    _windowTransform = windowTransform(ICamera::width(), ICamera::height(),
                                       fov_rad, worldToScreen);
    _zNear = _windowTransform(2, 3);
  }

  FrustumCamera::~FrustumCamera()
  {
  }

  bool FrustumCamera::isValid() const
  {
    return !_windowTransform.isZero();
  }

  Ray FrustumCamera::ray(const size_t x, const size_t y, const SamplerPtr& sampler) const
  {
    if( sampler->isRandom()  &&  _rLens > ZERO  &&  _zFocus < ZERO ) {
      // (1) Primary Ray to Screen ///////////////////////////////////////////

      const Vertex ps = makeRay(_windowTransform, x, y, sampler).origin();

      // (2) Primary Ray's Focus /////////////////////////////////////////////

      const Vertex pf{ ps.x*_zFocus/_zNear, ps.y*_zFocus/_zNear, _zFocus};

      // (3) Sample Disc /////////////////////////////////////////////////////

      const Vertex pl = _rLens*ConcentricDisk::sample(sampler->sample2D());

      // (4) Image Ray's Direction ///////////////////////////////////////////

      const Direction di = geom::to_direction(n4::direction(pl, pf));

      // (5) Image Ray ///////////////////////////////////////////////////////

      const real_t tNear = _zNear/di.z;
      const Vertex    oi{ pl.x + tNear*di.x, pl.y + tNear*di.y, _zNear};
      const Ray     rayi{ oi, di };

      // Done! ///////////////////////////////////////////////////////////////

      return rayi;
    }
    return makeRay(_windowTransform, x, y, sampler);
  }

  CameraPtr FrustumCamera::create(const size_t width, const size_t height,
                                  const real_t fov_rad, const real_t worldToScreen,
                                  const real_t aperture, const real_t focus)
  {
    return std::make_unique<FrustumCamera>(width, height, fov_rad, worldToScreen, aperture, focus);
  }

  CameraPtr FrustumCamera::create(const size_t width, const size_t height,
                                  const RenderOptions& options)
  {
    return create(width, height, options.fov_rad, options.worldToScreen,
                  options.aperture, options.focus);
  }

  ////// private /////////////////////////////////////////////////////////////

  Matrix FrustumCamera::windowTransform(const size_t width, const size_t height,
                                        const real_t fov_rad, const real_t worldToScreen)
  {
    // Screen
    const real_t wscr = static_cast<real_t>(width);
    const real_t hscr = static_cast<real_t>(height);

    const Matrix viewportInv = // glViewport()
        n4::translate(-1, 1, 0)*
        n4::scale(TWO/wscr, -TWO/hscr, 1);

    // World
    const real_t    a = wscr/hscr;
    const real_t hwrl = worldToScreen;
    const real_t wwrl = a*hwrl;

    // Frustum
    const real_t l = -wwrl/TWO;
    const real_t r =  wwrl/TWO;
    const real_t b = -hwrl/TWO;
    const real_t t =  hwrl/TWO;
    const real_t n = (r - l)/TWO/n4::tan(fov_rad/TWO);

    const Matrix frustumInv = // glFrustum()
        n4::translate((r + l)/TWO, (t + b)/TWO, -n)*
        n4::scale((r - l)/TWO, (t - b)/TWO, 1);

    return frustumInv*viewportInv;
  }

} // namespace rt
