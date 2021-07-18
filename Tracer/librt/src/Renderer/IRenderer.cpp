/****************************************************************************
** Copyright (c) 2021, Carsten Schmidt. All rights reserved.
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

#include "rt/Renderer/IRenderer.h"

#include "rt/Renderer/RenderLoop.h"

namespace rt {

  ////// public //////////////////////////////////////////////////////////////

  IRenderer::IRenderer(const RenderOptions& options) noexcept
  {
    setOptions(options);
  }

  IRenderer::~IRenderer() noexcept
  {
  }

  const RenderOptions& IRenderer::options() const
  {
    return _options;
  }

  void IRenderer::setOptions(const RenderOptions& options)
  {
    _options = options;

    _options.maxDepth = std::max<uint_t>(2, _options.maxDepth);

    const Transform xfrmCW{Transform::rotateZYXbyPI2(0, 0, -1)};

    // Transform camera setup from world to camera coordinates...
    const Vertex         eyeC = xfrmCW*_options.eye;
    const Vertex      lookAtC = xfrmCW*_options.lookAt;
    const Direction cameraUpC = xfrmCW*_options.cameraUp;

    /*
     * NOTE:
     * The view transform is comprised of two transforms:
     * 1. camera coordinates are aligned using a look-at transform in camera space
     * 2. camera space is transformed to world space
     */
    _view = xfrmCW.inverse()*Transform::lookAt(eyeC, lookAtC, cameraUpC);
  }

  Image IRenderer::render(size_t y0, size_t y1, const Scene& scene,
                          const CameraPtr& camera, const SamplerPtr& sampler) const
  {
    Image image = createImage(y0, y1, camera);
    if( image.isEmpty() ) {
      return Image();
    }

    if( sampler->isRandom() ) {
      render_loop(image, y0, [&](const size_t x, const size_t y) -> Color {
        Color color;
        for(size_t s = 0; s < sampler->numSamplesPerPixel(); s++) {
          const Color Li = radiance(_view*camera->ray(x, y, sampler), scene, sampler);
          color += Li;
        }
        color /= static_cast<real_t>(sampler->numSamplesPerPixel());
        return color;
      });
    } else {
      render_loop(image, y0, [&](const size_t x, const size_t y) -> Color {
        const Color Li = radiance(_view*camera->ray(x, y, sampler), scene, sampler);
        return Li;
      });
    }

    return image;
  }

  ////// private /////////////////////////////////////////////////////////////

  Image IRenderer::createImage(size_t& y0, size_t& y1, const CameraPtr& camera)
  {
    if( !camera  ||  camera->width() < 1  ||  camera->height() < 1 ) {
      return Image();
    }
    y0 = std::clamp<size_t>(y0, 0, camera->height());
    y1 = std::clamp<size_t>(y1, 0, camera->height());
    if( y0 == y1  ||  y0 >= camera->height() ) {
      return Image();
    }
    return Image(camera->width(), y1 - y0);
  }

} // namespace rt
