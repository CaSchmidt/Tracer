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

#include "rt/BxDF/PhongBRDF.h"

#include "geom/Shading.h"

namespace rt {

  PhongBRDF::PhongBRDF() noexcept
    : IBxDF(Flags(Specular | Reflection))
  {
    setSpecular(0);
  }

  PhongBRDF::~PhongBRDF()
  {
  }

  real_t PhongBRDF::specular() const
  {
    return _spec;
  }

  void PhongBRDF::setSpecular(const real_t spec)
  {
    _spec = std::max<real_t>(0, spec);
    _norm = (_spec + TWO)/TWO/PI;
  }

  bool PhongBRDF::isShadowCaster() const
  {
    return true;
  }

  Color PhongBRDF::eval(const Direction& wo, const Direction& wi) const
  {
    const Direction     R = geom::shading::reflect(wi);
    const real_t cosAlpha = n4::dot(R, wo);
    if( cosAlpha < ZERO  ||  _spec < ONE ) {
      return Color();
    }
    return _color*_norm*n4::pow(cosAlpha, _spec);
  }

  Color PhongBRDF::sample(const BxDFdata& /*input*/, Direction& wi) const
  {
    wi = Direction();
    return Color();
  }

} // namespace rt
