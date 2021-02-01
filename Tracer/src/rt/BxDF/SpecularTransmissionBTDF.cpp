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
#include "rt/BxDF/Shading.h"

namespace rt {

  SpecularTransmissionBTDF::SpecularTransmissionBTDF()
    : IBxDF(Type(Specular | Transmission))
  {
    setColor({1, 1, 1});
    setRefraction(1);
  }

  SpecularTransmissionBTDF::~SpecularTransmissionBTDF()
  {
  }

  Color SpecularTransmissionBTDF::color() const
  {
    return _color;
  }

  void SpecularTransmissionBTDF::setColor(const Color& c)
  {
    _color = n4::clamp(c, 0, 1);
  }

  real_t SpecularTransmissionBTDF::refraction() const
  {
    return _etat;
  }

  void SpecularTransmissionBTDF::setRefraction(const real_t etat)
  {
    _etat = std::max<real_t>(1, etat);
  }

  bool SpecularTransmissionBTDF::isShadowCaster() const
  {
    return false;
  }

  Color SpecularTransmissionBTDF::eval(const Direction& /*wo*/, const Direction& /*wi*/) const
  {
    return Color();
  }

  Color SpecularTransmissionBTDF::sample(const BxDFdata& input, Direction& wi) const
  {
    const bool entering = shading::isSameHemisphere(input.wo);
    const real_t nz = entering
        ? +ONE
        : -ONE;
    const real_t eta = entering
        ? input.etai/_etat
        : _etat/input.etai;
    const real_t kR = geom::optics::dielectric(shading::absCosTheta(input.wo), eta);
    const real_t kT = ONE - kR;
    if( kT <= ZERO ) {
      wi = Direction();
      return Color();
    }
    wi = shading::refract(input.wo, nz, eta);
    return kT*_color/shading::absCosTheta(wi);
  }

} // namespace rt
