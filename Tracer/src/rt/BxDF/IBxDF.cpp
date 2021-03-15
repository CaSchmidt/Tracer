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

#include <N4/Util.h>

#include "rt/BxDF/IBxDF.h"

#include "geom/Shading.h"
#include "rt/Object/SurfaceInfo.h"
#include "rt/Sampler/Sampling.h"

namespace rt {

  ////// BxDFinputs //////////////////////////////////////////////////////////

  BxDFdata::BxDFdata(const Ray& ray, const SurfaceInfo& sinfo, const real_t etaA) noexcept
    : etaA{etaA}
  {
    tex = sinfo.texCoord2D();

    xfrmWS = n4::util::frameFromZ(sinfo.N);
    xfrmSW = xfrmWS.transpose();

    wo = -toShading(ray.direction());
  }

  ////// IBxDF ///////////////////////////////////////////////////////////////

  IBxDF::IBxDF(const Flags flags) noexcept
    : _flags{flags}
  {
  }

  IBxDF::~IBxDF()
  {
  }

  Color IBxDF::color() const
  {
    return _color;
  }

  void IBxDF::setColor(const Color& c)
  {
    _color = n4::clamp(c, 0, 1);
  }

  IBxDF::Flags IBxDF::flags() const
  {
    return _flags;
  }

  bool IBxDF::matchFlags(const Flags f) const
  {
    // NOTE: 'f' can be a superset of '_flags'!
    return (_flags & f) == _flags;
  }

  real_t IBxDF::pdf(const Direction& wo, const Direction& wi) const
  {
    return geom::shading::isSameHemisphere(wo, wi)
        ? CosineHemisphere::pdf(geom::shading::absCosTheta(wi))
        : 0;
  }

  Color IBxDF::sample(const BxDFdata& input, Direction *wi, real_t *pdf) const
  {
    *wi = CosineHemisphere::sample(input.xi);
    if( input.wo.z < ZERO ) {
      wi->z *= -1;
    }
    if( pdf != nullptr ) {
      *pdf = IBxDF::pdf(input.wo, *wi);
    }
    return eval(input.wo, *wi);
  }

} // namespace rt
