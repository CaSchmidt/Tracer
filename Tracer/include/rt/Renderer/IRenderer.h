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

#ifndef IRENDERER_H
#define IRENDERER_H

#include "Image.h"
#include "rt/Camera/ICamera.h"
#include "rt/Renderer/RenderOptions.h"
#include "rt/Sampler/ISampler.h"
#include "rt/Scene/Scene.h"

namespace rt {

  using RendererPtr = std::unique_ptr<class IRenderer>;

  class IRenderer {
  public:
    IRenderer() noexcept;
    virtual ~IRenderer() noexcept;

    void clear();

    const ICamera *camera() const;
    void setCamera(CameraPtr& camera);

    const RenderOptions& options() const;
    bool setOptions(const RenderOptions& options);

    const Scene& scene() const;
    void setScene(Scene& scene);

    Image render(size_t y0, size_t y1, const SamplerPtr& sampler, const size_t samples) const;

  private:
    Image createImage(size_t& y0, size_t& y1) const;
    virtual Color radiance(const Ray& ray, const unsigned int depth = 0) const = 0;

    CameraPtr     _camera;
    RenderOptions _options{};
    Scene         _scene{};
    Transform     _view{};
  };

} // namespace rt

#endif // IRENDERER_H
