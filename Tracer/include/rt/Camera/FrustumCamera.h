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
    FrustumCamera();
    ~FrustumCamera();

    bool setup(const RenderOptions& options);
    bool setup(const real_t fov_rad, const real_t worldToScreen,
               const real_t aperture = 0, const real_t focus = 0);

    Image render(const size_t width, const size_t height,
                 size_t y0, size_t y1,
                 const IRenderer *renderer, const size_t samples = 0) const;

  private:
    Vertex sampleDisc() const;
    static Matrix windowTransform(const size_t width, const size_t height,
                                  const real_t fov_rad, const real_t worldToScreen);

    real_t _fov_rad{};
    real_t _rLens{};
    real_t _worldToScreen{};
    real_t _zFocus{};
  };

} // namespace rt

#endif // FRUSTUMCAMERA_H
