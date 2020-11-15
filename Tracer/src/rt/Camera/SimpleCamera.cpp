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

#include "rt/Camera/RenderLoop.h"
#include "rt/Renderer.h"

namespace rt {

  ////// public //////////////////////////////////////////////////////////////

  SimpleCamera::SimpleCamera()
    : ICamera()
  {
    setup(0);
  }

  SimpleCamera::~SimpleCamera()
  {
  }

  void SimpleCamera::setup(const real_T fov_rad)
  {
    _fov_rad = fov_rad;
  }

  Image SimpleCamera::render(const std::size_t width, const std::size_t height,
                             std::size_t y0, std::size_t y1,
                             const Renderer& renderer, const std::size_t samples) const
  {
    Image image = create_image(width, height, y0, y1);
    if( image.isEmpty() ) {
      return Image();
    }

    const Matrix3f W = windowTransform(width, height);

    if( samples > 1 ) {
      render_loop(image, y0, [&](const std::size_t x, const std::size_t y) -> Color3f {
        Color3f color;
        for(std::size_t s = 0; s < samples; s++) {
          color += cs::clamp(renderer.castCameraRay(ray(W, x, y, true)), 0, 1);
        }
        color /= static_cast<real_T>(samples);
        return color;
                  });
    } else {
      render_loop(image, y0, [&](const std::size_t x, const std::size_t y) -> Color3f {
                    return renderer.castCameraRay(ray(W, x, y));
                  });
    }

    return image;
  }

  ////// private /////////////////////////////////////////////////////////////

  Rayf SimpleCamera::ray(const Matrix3f& W, const std::size_t x, const std::size_t y,
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

  /*
   * NOTE:
   * Cf. to Scratchapixel v2.0 for the derivation of the window transform:
   * https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-generating-camera-rays/generating-camera-rays
   */

  Matrix3f SimpleCamera::windowTransform(const std::size_t width, const std::size_t height) const
  {
    const real_T w = static_cast<real_T>(width);
    const real_T h = static_cast<real_T>(height);
    const real_T a = w/h;
    const real_T n = csTan(_fov_rad/TWO);
    return Matrix3f{ TWO*a*n/w, 0, -a*n, 0, -TWO*n/h, n, 0, 0, -1 };
  }

} // namespace rt
