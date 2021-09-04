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

#include "pt/BSDF/Dielectric.h"

#include "geom/Optics.h"
#include "geom/Shading.h"

namespace pt {

  ////// public //////////////////////////////////////////////////////////////

  Dielectric::Dielectric(const rt::real_t eta) noexcept
  {
    setEta(eta);
  }

  Dielectric::~Dielectric() noexcept
  {
  }

  rt::Color Dielectric::eval(const rt::Direction& /*wo*/, const rt::Direction& /*wi*/) const
  {
    return rt::Color(0);
  }

  rt::real_t Dielectric::pdf(const rt::Direction& /*wo*/, const rt::Direction& /*wi*/) const
  {
    return 0;
  }

  rt::Color Dielectric::sample(const rt::Direction& wo, rt::Direction *wi, const rt::Sample2D& xi) const
  {
    constexpr rt::real_t TODO_etaA = 1;

    SAMPLES_2D(xi);

    /*
     * NOTE:
     * Using ThetaI is somewhat ambiguous as it can either represent the
     * angle of reflection ThetaR or the angle of transmission ThetaT
     * respectively.
     * Therefore we will avoid using ThetaI and use ThetaO and ThetaT when
     * appropriate.
     */

    // (1) Geometry Setup ////////////////////////////////////////////////////

    const rt::real_t cosTo = geom::shading::cosTheta(wo);
    const rt::real_t   eta = geom::optics::boundaryEta(cosTo, TODO_etaA, _etaB);

    // (2) Specular Reflection/Transmission //////////////////////////////////

    /*
     * NOTE:
     * xi := [0,0.5) -> Reflection
     * xi := [0.5,1) -> Transmission
     */
    const bool is_reflection = xi1 < rt::ONE_HALF;

    // (3) Implementation ////////////////////////////////////////////////////

    rt::real_t kResult = 0;
    if( is_reflection ) { // Specular Reflection
      *wi     = geom::shading::reflect(wo);
      kResult = geom::optics::dielectric(cosTo, eta);

    } else { // Specular Transmission
      *wi = geom::shading::refract(wo, eta);
      if( wi->isZero() ) {
        kResult = 0; // Total internal reflection
      } else {
        const rt::real_t cosTt = geom::shading::cosTheta(*wi);

        /*
         * NOTE:
         * ThetaO is directed opposite to ThetaT, so we need to take
         * the reciprocal of eta when calling optics::dielectric() to
         * avoid re-computing a boundary eta.
         */
        kResult  = rt::ONE - geom::optics::dielectric(cosTt, rt::ONE/eta);
        kResult *= eta*eta;
      }

    }

    return _color*kResult;
  }

  rt::real_t Dielectric::eta() const
  {
    return _etaB;
  }

  void Dielectric::setEta(const rt::real_t eta)
  {
    _etaB = std::max<rt::real_t>(1, eta);
  }

  BSDFPtr Dielectric::create(const rt::real_t eta)
  {
    if( eta < rt::ONE ) {
      return BSDFPtr();
    }
    return std::make_unique<Dielectric>(eta);
  }

} // namespace pt
