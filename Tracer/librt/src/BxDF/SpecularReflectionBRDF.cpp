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

#include "rt/BxDF/SpecularReflectionBRDF.h"

#include "geom/Optics.h"
#include "geom/Shading.h"

namespace rt {

  SpecularReflectionBRDF::SpecularReflectionBRDF() noexcept
    : IBxDF(Flags(Specular | Reflection))
  {
    setRefraction(1);
  }

  SpecularReflectionBRDF::~SpecularReflectionBRDF()
  {
  }

  real_t SpecularReflectionBRDF::refraction() const
  {
    return _etaIn;
  }

  void SpecularReflectionBRDF::setRefraction(const real_t eta)
  {
    _etaIn = std::max<real_t>(1, eta);
  }

  Color SpecularReflectionBRDF::eval(const Direction& /*wo*/, const Direction& /*wi*/) const
  {
    return Color();
  }

  real_t SpecularReflectionBRDF::pdf(const Direction& /*wo*/, const Direction& /*wi*/) const
  {
    return 0;
  }

  Color SpecularReflectionBRDF::sample(const Direction& wo, Direction *wi, const Sample2D& /*xi*/, real_t *pdf) const
  {
    constexpr real_t TODO_etaOut = 1;

    const real_t cosTo = geom::shading::cosTheta(wo);
    const real_t    kR = geom::optics::dielectric(cosTo, TODO_etaOut, _etaIn);
    *wi = geom::shading::reflect(wo);
    if( pdf != nullptr ) {
      *pdf = 1;
    }
    return kR*_color/geom::shading::absCosTheta(*wi);
  }

} // namespace rt
