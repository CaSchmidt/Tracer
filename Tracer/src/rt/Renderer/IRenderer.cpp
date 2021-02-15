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

namespace rt {

  IRenderer::IRenderer() noexcept
  {
  }

  IRenderer::~IRenderer() noexcept
  {
  }

  Color IRenderer::castCameraRay(const Ray& ray) const
  {
    return radiance(_view*ray);
  }

  void IRenderer::clear()
  {
    _options = RenderOptions();
    _view    = Transform();
    _scene.clear();
  }

  bool IRenderer::initialize(const RenderOptions& options)
  {
    _options = options;

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

    return true;
  }

  const RenderOptions& IRenderer::options() const
  {
    return _options;
  }

  const Scene& IRenderer::scene() const
  {
    return _scene;
  }

  void IRenderer::setScene(Scene& scene)
  {
    _scene = std::move(scene);
  }

} // namespace rt
