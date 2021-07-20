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

#ifndef FRUSTUMCAMERA_H
#define FRUSTUMCAMERA_H

#include "rt/Camera/ICamera.h"

namespace rt {

  struct RenderOptions;

  class FrustumCamera : public ICamera {
  public:
    FrustumCamera(const size_t width, const size_t height,
                  const real_t fov_rad, const real_t worldToScreen,
                  const real_t aperture = ZERO, const real_t focus = ZERO);
    ~FrustumCamera();

    bool isValid() const;

    Ray ray(const size_t x, const size_t y, const SamplerPtr& sampler) const;

    static CameraPtr create(const size_t width, const size_t height,
                            const real_t fov_rad, const real_t worldToScreen,
                            const real_t aperture = ZERO, const real_t focus = ZERO);

    static CameraPtr create(const size_t width, const size_t height,
                            const RenderOptions& options);

  private:
    static Matrix windowTransform(const size_t width, const size_t height,
                                  const real_t fov_rad, const real_t worldToScreen);

    real_t _rLens{};
    Matrix _windowTransform{};
    real_t _zFocus{};
    real_t _zNear{};
  };

} // namespace rt

#endif // FRUSTUMCAMERA_H
