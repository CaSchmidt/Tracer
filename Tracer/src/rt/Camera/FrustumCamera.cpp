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

#include "rt/Camera/RenderLoop.h"
#include "rt/Renderer.h"

namespace rt {

  ////// public //////////////////////////////////////////////////////////////

  FrustumCamera::FrustumCamera()
    : ICamera()
  {
  }

  FrustumCamera::~FrustumCamera()
  {
  }

  bool FrustumCamera::setup(const real_T fov_rad, const real_T worldToScreen,
                            const real_T aperture, const real_T focus)
  {
    _fov_rad = _worldToScreen = 0;

    if( !isValidFoV(fov_rad)  ||  worldToScreen <= ZERO ) {
      return false;
    }

    _fov_rad = fov_rad;
    _worldToScreen = worldToScreen;

    if( aperture >= ZERO  &&  focus >= ZERO ) {
      _rLens  = aperture/TWO;
      _zFocus = -focus;
    } else {
      _rLens = _zFocus = 0;
    }

    return true;
  }

  Image FrustumCamera::render(const std::size_t width, const std::size_t height,
                              std::size_t y0, std::size_t y1,
                              const Renderer& renderer, const std::size_t samples) const
  {
    Image image = create_image(width, height, y0, y1);
    if( image.isEmpty() ) {
      return Image();
    }

    const Matrix3f W = windowTransform(width, height, _fov_rad, _worldToScreen);

    if( samples > 1 ) {
      if( _rLens > ZERO  &&  _zFocus < ZERO ) {
        const real_T _zNear = W(2, 2);
        render_loop(image, y0, [&](const std::size_t x, const std::size_t y) -> Color3f {
                      Color3f color;
                      for(std::size_t s = 0; s < samples; s++) {
                        // (1) Sample Disc ///////////////////////////////////

                        const Vertex3f pl = _rLens*sampleDisc();

                        // (2) Primary Ray to Screen /////////////////////////

                        const Vertex3f ps = ray(W, x, y).origin();

                        // (3) Primary Ray's Focus ///////////////////////////

                        const Vertex3f pf{ ps.x*_zFocus/_zNear, ps.y*_zFocus/_zNear, _zFocus};

                        // (4) Image Ray's Direction /////////////////////////

                        const Normal3f di = to_normal(cs::direction(pl, pf));

                        // (5) Image Ray /////////////////////////////////////

                        const real_T tNear = _zNear/di.z;
                        const Vertex3f  oi{ pl.x + tNear*di.x, pl.y + tNear*di.y, _zNear};
                        const Rayf    rayi{ oi, di };

                        // (6) Cast Ray //////////////////////////////////////

                        color += cs::clamp(renderer.castCameraRay(rayi), 0, 1);
                      }
                      color /= static_cast<real_T>(samples);
                      return color;
                    });
      } else {
        render_loop(image, y0, [&](const std::size_t x, const std::size_t y) -> Color3f {
          Color3f color;
          for(std::size_t s = 0; s < samples; s++) {
            color += cs::clamp(renderer.castCameraRay(ray(W, x, y, true)), 0, 1);
          }
          color /= static_cast<real_T>(samples);
          return color;
        });
      }
    } else {
      render_loop(image, y0, [&](const std::size_t x, const std::size_t y) -> Color3f {
        return renderer.castCameraRay(ray(W, x, y));
      });
    }

    return image;
  }

  ////// private /////////////////////////////////////////////////////////////

  Vertex3f FrustumCamera::sampleDisc() const
  {
    Vertex3f v;
    do {
      v = TWO*Vertex3f{rand(), rand(), 0} - Vertex3f{1, 1, 0};
    } while( cs::dot(v, v) >= ONE );
    return v;
  }

  Matrix3f FrustumCamera::windowTransform(const std::size_t width, const std::size_t height,
                                          const real_T fov_rad, const real_T worldToScreen)
  {
    // Screen
    const real_T wscr = static_cast<real_T>(width);
    const real_T hscr = static_cast<real_T>(height);

    const Matrix3f viewportInv{ // glViewport()
      TWO/wscr, 0, -1,
          0, -TWO/hscr, 1,
          0, 0, 1
    };

    // World
    const real_T a = wscr/hscr;
    const real_T hwrl = worldToScreen;
    const real_T wwrl = a*hwrl;

    // Frustum
    const real_T l = -wwrl/TWO;
    const real_T r =  wwrl/TWO;
    const real_T b = -hwrl/TWO;
    const real_T t =  hwrl/TWO;
    const real_T n = (r - l)/TWO/csTan(fov_rad/TWO);

    const Matrix3f frustumInv{ // glFrustum()
      (r - l)/TWO, 0, (r + l)/TWO,
          0, (t - b)/TWO, (t + b)/TWO,
          0, 0, -n
    };

    return frustumInv*viewportInv;
  }

} // namespace rt
