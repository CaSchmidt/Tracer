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

#include "rt/Renderer/RenderUtils.h"

#include "geom/Shading.h"
#include "rt/BxDF/IBxDF.h"
#include "rt/Material/BSDFdata.h"
#include "rt/Object/IObject.h"
#include "rt/Object/SurfaceInfo.h"
#include "rt/Sampler/Sampling.h"
#include "rt/Scene/Scene.h"

namespace rt {

  Color estimateDirectLighting(const SurfaceInfo& surface, const Sample2D& xiSurface,
                               const LightPtr& light, const Sample2D& xiLight,
                               const Scene& scene, const SamplerPtr& sampler,
                               const bool is_specular)
  {
    const BSDF *bsdf = surface->material()->bsdf();

    const IBxDF::Flags bsdf_flags = is_specular
        ? IBxDF::AllFlags
        : IBxDF::Flags(IBxDF::AllFlags & ~IBxDF::Specular);
    const BSDFdata bsdf_data(surface);

    Color Ld;

    // (1) Sample Light Source with Multiple Importance Sampling /////////////

    real_t   pdfLight = 0;
    Ray           vis{};
    Direction      wi{};
    const Color Li = light->sampleLi(surface, &wi, xiLight, &pdfLight, &vis);

    if( pdfLight > ZERO  &&  !Li.isZero() ) {
      const real_t   absCosTi = geom::absDot(wi, surface.N);
      const Color           f = bsdf->eval(bsdf_data, wi, bsdf_flags)*absCosTi;
      const real_t pdfSurface = bsdf->pdf(bsdf_data, wi, bsdf_flags);

      /*
       * NOTE: Compute light's contribution, IFF
       * 1. Surface DOES NOT reflect black
       * AND
       * 2. Light is visible to the surface
       */
      if( !f.isZero()  &&  !scene.intersect(vis) ) {
        if( light->isDeltaLight() ) {
          Ld += f*Li/pdfLight;
        } else {
          const real_t w = sampling::powerHeuristic(1, pdfLight, 1, pdfSurface);
          Ld += f*Li*w/pdfLight;
        }
      }
    }

    // (2) Sample BSDF with Multiple Importance Sampling /////////////////////

    if( !light->isDeltaLight() ) {
      // TODO...
    }

    return Ld;
  }

  Color uniformSampleAllLights(const SurfaceInfo& surface,
                               const Scene& scene, const SamplerPtr& sampler)
  {
    Color L;
    for(const LightPtr& light : scene.lights()) {
      const size_t numSamples = light->numSamples();
      Color Ld;
      for(size_t s = 0; s < numSamples; s++) {
        Ld += estimateDirectLighting(surface, sampler->sample2D(),
                                     light, sampler->sample2D(),
                                     scene, sampler);
      }
      L += Ld/real_t(numSamples);
    }
    return L;
  }

  Color uniformSampleOneLight(const SurfaceInfo& surface,
                              const Scene& scene, const SamplerPtr& sampler)
  {
    const Lights& lights = scene.lights();

    if( lights.size() < 1 ) {
      return Color();
    }

    const size_t choice = sampling::choose(sampler->sample(), lights.size());
    const LightPtr& light = *std::next(lights.cbegin(), choice);

    const size_t numLights = lights.size();

    return real_t(numLights)*estimateDirectLighting(surface, sampler->sample2D(),
                                                    light, sampler->sample2D(),
                                                    scene, sampler);
  }

} // namespace rt
