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

#include "pt/BSDF/Diffuse.h"

#include "geom/Shading.h"
#include "rt/Sampler/Sampling.h"

namespace pt {

  Diffuse::Diffuse() noexcept
  {
  }

  Diffuse::~Diffuse() noexcept
  {
  }

  rt::Color Diffuse::eval(const rt::Direction& wo, const rt::Direction& wi) const
  {
    const rt::real_t cosTo = geom::shading::cosTheta(wo);
    const rt::real_t cosTi = geom::shading::cosTheta(wi);
    if( cosTo <= rt::ZERO  ||  cosTi <= rt::ZERO ) {
      return rt::Color(0);
    }
    return _color/rt::PI;
  }

  rt::real_t Diffuse::pdf(const rt::Direction& wo, const rt::Direction& wi) const
  {
    const rt::real_t cosTo = geom::shading::cosTheta(wo);
    const rt::real_t cosTi = geom::shading::cosTheta(wi);
    if( cosTo <= rt::ZERO  ||  cosTi <= rt::ZERO ) {
      return 0;
    }
    return rt::CosineHemisphere::pdf(cosTi);
  }

  rt::Color Diffuse::sample(const rt::Direction& wo, rt::Direction *wi, const rt::Sample2D& xi) const
  {
    const rt::real_t cosTo = geom::shading::cosTheta(wo);
    if( cosTo <= rt::ZERO ) {
      return rt::Color(0);
    }
    *wi = rt::CosineHemisphere::sample(xi);
    /*
     * Cosine-Weighted Hemisphere Sampling of Diffuse Materials:
     *
     *   f = albedo/PI -> diffuse material
     * pdf = cosTi/PI  -> cosine-weighted hemisphere sampling
     *
     * Monte Carlo Rendering Equation w/o Incident Radiance:
     *
     * f*cosTi/pdf = (albedo/PI)*cosTi/(cosTi/PI) = albedo
     */
    return _color;
  }

  BSDFPtr Diffuse::create()
  {
    return std::make_unique<Diffuse>();
  }

} // namespace pt
