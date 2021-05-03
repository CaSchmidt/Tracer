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
                                  const uint_t depth) const
  {
    const Scene& scene = WhittedRenderer::scene();

    SurfaceInfo ref;
    if( !scene.intersect(&ref, ray) ) {
      return options().backgroundColor;
    }

    Color Lo;

    Lo += ref.Le(ref.wo); // Account for emissive lighting.

    for(const LightPtr& light : scene.lights()) {
      real_t pdfLight{0};
      Ray         vis{};
      Direction    wi{};
      const Color Li = light->sampleLi(ref, &wi, sampler->sample2D(), &pdfLight, &vis);
      if( pdfLight <= ZERO  ||  Li.isZero() ) { // No light contribution.
        continue;
      }

      if( scene.intersect(vis) ) { // Light is occluded by scene.
        continue;
      }

      const Color         f = ref->material()->bsdf()->eval(ref, wi);
      const real_t absCosTi = geom::absDot(wi, ref.N);
      if( absCosTi == ZERO  ||  f.isZero() ) {
        continue;
      }

      Lo += f*Li*absCosTi/pdfLight;
    }

    if( depth + 1 < options().maxDepth ) {
      Lo += specularReflectOrTransmit(ref, sampler, depth, false);
      Lo += specularReflectOrTransmit(ref, sampler, depth, true);
    }

    return Lo;
  }

} // namespace rt
