/****************************************************************************
** Copyright (c) 2019, Carsten Schmidt. All rights reserved.
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

#include "rt/Light/PointLight.h"

#include "rt/Object/SurfaceInfo.h"

namespace rt {

  ////// public //////////////////////////////////////////////////////////////

  PointLight::PointLight(const Transform& lightToWorld, const Color& I) noexcept
    : ILight(DeltaPosition, lightToWorld)
    , _I{I}
  {
    _pW = toWorld(Vertex{0, 0, 0});
  }

  PointLight::~PointLight() noexcept
  {
  }

  real_t PointLight::pdfLi(const SurfaceInfo& /*surface*/, const Direction& /*wi*/) const
  {
    return 0;
  }

  Color PointLight::sampleLi(const SurfaceInfo& surface, Direction *wi,
                             const Sample2D& /*xi*/, real_t *pdf, Ray *vis) const
  {
    const real_t r = n4::distance(surface.P, _pW);
    *wi  = geom::to_direction(n4::direction(surface.P, _pW));
    if( pdf != nullptr ) {
      *pdf = 1;
    }
    if( vis != nullptr ) {
      *vis = surface.ray(*wi, TRACE_BIAS, r);
    }
    return _I*attenuation(r);
  }

  LightPtr PointLight::create(const Transform& lightToWorld, const Color& I)
  {
    return std::make_unique<PointLight>(lightToWorld, I);
  }

  ////// private /////////////////////////////////////////////////////////////

  real_t PointLight::attenuation(const real_t r) const
  {
    const real_t rL = std::max<real_t>(r, ONE);
    return ONE/rL/rL;
  }

} // namespace rt
