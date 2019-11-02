/****************************************************************************
** Copyright (c) 2019, Carsten Schmidt. All rights reserved.
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

#ifndef RENDERER_H
#define RENDERER_H

#include "rt/Camera.h"
#include "rt/Light/ILightSource.h"
#include "rt/Scene/IObject.h"

namespace rt {

  struct RenderOptions {
    RenderOptions() = default;

    Color3f      backgroundColor{};
    Vertex3f     eye{};
    Vertex3f     lookAt{};
    Normal3f     cameraUp{};
    dim_T        width{};
    dim_T        height{};
    real_T       fov_rad{};
    unsigned int maxDepth{15};
    real_T       globalRefraction{1};
  };

  class Renderer {
  public:
    Renderer() = default;
    ~Renderer() noexcept = default;

    void addLight(LightSourcePtr& light);

    bool initialize(const RenderOptions& options, Objects& scene);

    const RenderOptions& options() const;

    bool render(const dim_T y, uint8_t *row, const uint8_t samples = 1) const;

  private:
    // NOTE: All arguments passed to/returned from these methods are in WORLD coordinates!
    Color3f castRay(const Rayf& ray, const real_T tMax = MAX_REAL_T, const unsigned int depth = 0) const;
    Color3f shade(const SurfaceInfo& sinfo, const Normal3f& v) const;
    bool trace(SurfaceInfo& result, const Rayf& ray, const real_T tMax, const bool isShadowRay) const;

    Camera        _camera{};
    RenderOptions _options{};
    Transformf    _xfrmWC{}; // Camera -> World
    Objects       _scene{};
    LightSources  _lights{};
  };

} // namespace rt

#endif // RENDERER_H
