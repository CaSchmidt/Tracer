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
#include "rt/Light/IAreaLight.h"
#include "rt/Material/BSDFdata.h"
#include "rt/Object/IObject.h"
#include "rt/Object/SurfaceInfo.h"
#include "rt/Sampler/Sampling.h"
#include "rt/Scene/Scene.h"

namespace rt {

  namespace priv {

    Color multiSampleBSDF(const BSDFdata& ref_data, const IBxDF::Flags ref_flags,
                          const SurfaceInfo& ref, const Sample2D& xiRef,
                          const LightPtr& light,
                          const Scene& scene)
    {
      const BSDF *bsdf = ref->material()->bsdf();

      if( light->isDeltaLight() ) {
        return Color();
      }

      // (1) Sample BSDF /////////////////////////////////////////////////////

      real_t              pdfRef{};
      IBxDF::Flags sampled_flags{IBxDF::InvalidFlags};
      Direction               wi{};
      const Color         f = bsdf->sample(ref_data, &wi, xiRef, &pdfRef, ref_flags, &sampled_flags);
      const real_t absCosTi = geom::absDot(wi, ref.N);
      if( pdfRef <= ZERO  ||  absCosTi == ZERO  ||  f.isZero() ) {
        return Color();
      }

      // (2) Light Contribution //////////////////////////////////////////////

      real_t pdfLight = 0;
      real_t   weight = 1; // Disable MIS. Assumes specular reflection.
      if( !isSpecular(sampled_flags) ) {
        pdfLight = light->pdfLi(ref, wi);
        if( pdfLight <= ZERO ) {
          return Color();
        }
        weight = sampling::powerHeuristic(1, pdfRef, 1, pdfLight);
      }

      const Ray           ray = ref.ray(wi, TRACE_BIAS);
      SurfaceInfo   lightInfo{};
      const bool is_intersect = scene.intersect(&lightInfo, ray);
      const Color Li = is_intersect  &&  lightInfo->areaLight() == IAREALIGHT(light)
          ? lightInfo.Le(-wi)
          : Color(); // TODO: Add light's background radiance.
      if( Li.isZero() ) { // Light does not contribute...
        return Color();
      }

      // (3) Compute radiance with Multiple Importance Sampling (MIS) ////////

      return f*Li*absCosTi*weight/pdfRef;
    }

    Color multiSampleLight(const BSDFdata& ref_data, const IBxDF::Flags ref_flags,
                           const SurfaceInfo& ref,
                           const LightPtr& light, const Sample2D& xiLight,
                           const Scene& scene)
    {
      const BSDF *bsdf = ref->material()->bsdf();

      // (1) Sample light ////////////////////////////////////////////////////

      real_t pdfLight{};
      Ray         vis{};
      Direction    wi{};
      const Color Li = light->sampleLi(ref, &wi, xiLight, &pdfLight, &vis);
      if( pdfLight <= ZERO  ||  Li.isZero() ) {
        return Color();
      }

      // (2) Evaluate BSDF ///////////////////////////////////////////////////

      const Color         f = bsdf->eval(ref_data, wi, ref_flags);
      const real_t absCosTi = geom::absDot(wi, ref.N);
      const real_t   pdfRef = bsdf->pdf(ref_data, wi, ref_flags);

      /*
       * NOTE:
       * BSDF does not evaluate to black and 'ref' is visible to the light.
       */
      if( absCosTi == ZERO  ||  f.isZero()  ||  scene.intersect(vis) ) {
        return Color();
      }

      // (3) Compute radiance with Multiple Importance Sampling (MIS) ////////

      const real_t weight = !light->isDeltaLight()
          ? sampling::powerHeuristic(1, pdfLight, 1, pdfRef) // Apply MIS.
          : 1;                     // Disable MIS for delta distributions.

      return f*Li*absCosTi*weight/pdfLight;
    }

  } // namespace priv

  Color estimateDirectLighting(const BSDFdata& ref_data,
                               const SurfaceInfo& ref, const Sample2D& xiRef,
                               const LightPtr& light, const Sample2D& xiLight,
                               const Scene& scene, const bool do_specular)
  {
    const IBxDF::Flags ref_flags = do_specular
        ? IBxDF::AllFlags
        : IBxDF::Flags(IBxDF::AllFlags & ~IBxDF::Specular);

    Color Ld;

    Ld += priv::multiSampleLight(ref_data, ref_flags, ref, light, xiLight, scene);
    Ld += priv::multiSampleBSDF(ref_data, ref_flags, ref, xiRef, light, scene);

    return Ld;
  }

  Color uniformSampleAllLights(const BSDFdata& ref_data, const SurfaceInfo& ref,
                               const Scene& scene, const SamplerPtr& sampler)
  {
    Color L;
    for(const LightPtr& light : scene.lights()) {
      const size_t numSamples = light->numSamples();
      Color Ld;
      for(size_t s = 0; s < numSamples; s++) {
        Ld += estimateDirectLighting(ref_data,
                                     ref, sampler->sample2D(),
                                     light, sampler->sample2D(),
                                     scene);
      }
      L += Ld/real_t(numSamples);
    }
    return L;
  }

  Color uniformSampleOneLight(const BSDFdata& ref_data, const SurfaceInfo& ref,
                              const Scene& scene, const SamplerPtr& sampler)
  {
    const Lights& lights = scene.lights();

    if( lights.size() < 1 ) {
      return Color();
    }

    const size_t choice = sampling::choose(sampler->sample(), lights.size());
    const LightPtr& light = *std::next(lights.cbegin(), choice);

    const size_t numLights = lights.size();

    return real_t(numLights)*estimateDirectLighting(ref_data,
                                                    ref, sampler->sample2D(),
                                                    light, sampler->sample2D(),
                                                    scene);
  }

} // namespace rt
