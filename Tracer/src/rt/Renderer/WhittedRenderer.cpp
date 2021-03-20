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

#include "rt/Renderer/WhittedRenderer.h"

#include "geom/Shading.h"
#include "rt/Material/BSDFdata.h"
#include "rt/Object/SurfaceInfo.h"

namespace rt {

  ////// public //////////////////////////////////////////////////////////////

  WhittedRenderer::WhittedRenderer() noexcept
  {
  }

  WhittedRenderer::~WhittedRenderer() noexcept
  {
  }

  ////// private /////////////////////////////////////////////////////////////

  Color WhittedRenderer::radiance(const Ray& ray, const SamplerPtr& sampler,
                                  const unsigned int depth) const
  {
    const Scene& scene = WhittedRenderer::scene();

    SurfaceInfo info;
    if( !scene.trace(info, ray) ) {
      return options().backgroundColor;
    }

    const BSDFdata data(ray, info, options().globalRefraction, sampler);

    Color color;
    for(const LightSourcePtr& light : scene.lights()) {
      Ray       vis;
      Direction wiW;
      const Color Li = light->sampleLi(info, &wiW, nullptr, &vis);

      if( scene.trace(vis) ) {
        continue;
      }

      const Direction   wiS = data.toShading(wiW);
      const real_t absCosTi = geom::shading::absCosTheta(wiS);
      if( absCosTi <= ZERO ) {
        continue;
      }

      const Color fR = info->material()->bsdf()->eval(data, wiS);
      if( fR.isZero() ) {
        continue;
      }

      color += fR*Li*absCosTi;
    }

    if( depth + 1 < options().maxDepth ) {
      color += specularReflectOrTransmit(data, info, sampler, depth, false);
      color += specularReflectOrTransmit(data, info, sampler, depth, true);
    }

    return color;
  }

} // namespace rt
