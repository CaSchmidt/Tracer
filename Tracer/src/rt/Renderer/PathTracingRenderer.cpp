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

#include "rt/Renderer/PathTracingRenderer.h"

#include "rt/Object/SurfaceInfo.h"
#include "rt/Renderer/RenderUtils.h"

namespace rt {

  ////// public //////////////////////////////////////////////////////////////

  PathTracingRenderer::PathTracingRenderer() noexcept
  {
  }

  PathTracingRenderer::~PathTracingRenderer() noexcept
  {
  }

  ////// private /////////////////////////////////////////////////////////////

  Color PathTracingRenderer::radiance(const Ray& _ray, const SamplerPtr& sampler,
                                      const uint_t /*depth*/) const
  {
    const RenderOptions& options = PathTracingRenderer::options();
    const Scene&           scene = PathTracingRenderer::scene();

    Color              beta(1);
    Color                 L;
    Ray                 ray{_ray};
    bool is_specular_bounce = false;

    // Find next path vertex and accumulate contribution
    for(uint_t bounces = 0; ; bounces++) {
      // Intersect ray with scene and store intersection in 'ref'
      SurfaceInfo ref;
      const bool is_intersect = scene.intersect(&ref, ray);

      // Possibly add emitted light at intersection
      if( bounces == 0  ||  is_specular_bounce ) {
        // Add emitted light at path vertex or from the environment
        if( is_intersect ) {
          L += beta*ref.Le(ref.wo);
        } else {
          L += beta*options.backgroundColor;
        }
      }

      // Terminate path if ray escaped or maxDepth was reached
      if( !is_intersect  ||  bounces >= options.maxDepth ) {
        break;
      }

      // Sample illumination from lights to find path contribution
      L += beta*uniformSampleOneLight(ref, scene, sampler);

      // Sample BSDF to get new path direction
      const BSDF *bsdf = ref->material()->bsdf();

      real_t              pdfRef{0};
      IBxDF::Flags sampled_flags{IBxDF::InvalidFlags};
      Direction               wi;
      const Color         f = bsdf->sample(ref, &wi, sampler->sample2D(), &pdfRef,
                                           IBxDF::AllFlags, &sampled_flags);
      const real_t absCosTi = geom::absDot(wi, ref.N);
      if( pdfRef <= ZERO  ||  absCosTi == ZERO  ||  f.isZero() ) {
        break;
      }

      beta *= f*absCosTi/pdfRef;
      is_specular_bounce = isSpecular(sampled_flags);
      ray = ref.ray(wi);

      // Possibly terminate the path with Russian roulette
      if( bounces > 3 ) {
        const real_t q = std::max<real_t>(0.0625, ONE - beta.max());
        if( sampler->sample() < q ) {
          break;
        }
        beta /= ONE - q;
      }
    }

    return L;
  }

} // namespace rt
