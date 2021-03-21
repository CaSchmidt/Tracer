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

#include "rt/BxDF/SpecularTransmissionBTDF.h"

#include "geom/Optics.h"
#include "geom/Shading.h"

namespace rt {

  SpecularTransmissionBTDF::SpecularTransmissionBTDF()
    : IBxDF(Flags(Specular | Transmission))
  {
    setRefraction(1);
  }

  SpecularTransmissionBTDF::~SpecularTransmissionBTDF()
  {
  }

  real_t SpecularTransmissionBTDF::refraction() const
  {
    return _etaB;
  }

  void SpecularTransmissionBTDF::setRefraction(const real_t etaB)
  {
    _etaB = std::max<real_t>(1, etaB);
  }

  bool SpecularTransmissionBTDF::isShadowCaster() const
  {
    return false;
  }

  Color SpecularTransmissionBTDF::eval(const Direction& /*wo*/, const Direction& /*wi*/) const
  {
    return Color();
  }

  real_t SpecularTransmissionBTDF::pdf(const Direction& /*wo*/, const Direction& /*wi*/) const
  {
    return 0;
  }

  Color SpecularTransmissionBTDF::sample(const Direction& wo, Direction *wi, const Sample2D& /*xi*/, real_t *pdf) const
  {
    constexpr real_t TODO_etaA = 1;

    if( pdf != nullptr ) {
      *pdf = 0;
    }

    // (1) Compute Perfect Specular Refraction ///////////////////////////////

    *wi = geom::shading::refract(wo, TODO_etaA, _etaB);
    if( wi->isZero() ) {
      *wi = Direction();
      return Color();
    }

    // (2) Compute Transmitted Energy ////////////////////////////////////////

    const real_t kR = geom::optics::dielectric(*wi, TODO_etaA, _etaB);
    const real_t kT = ONE - kR;
    if( kT <= ZERO ) {
      *wi = Direction();
      return Color();
    }

    // (3) Compute Outputs ///////////////////////////////////////////////////

    if( pdf != nullptr ) {
      *pdf = 1;
    }
    return kT*_color/geom::shading::absCosTheta(*wi);
  }

} // namespace rt
