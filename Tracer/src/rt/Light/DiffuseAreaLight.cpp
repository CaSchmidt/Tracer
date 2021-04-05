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

#include "rt/Light/DiffuseAreaLight.h"

#include "rt/Object/IObject.h"
#include "rt/Object/SurfaceInfo.h"

namespace rt {

  DiffuseAreaLight::DiffuseAreaLight(const IObject *object, const Color& Lemit) noexcept
    : IAreaLight(object->objectToWorld())
    , _Lemit(Lemit)
    , _object{object}
  {
  }

  DiffuseAreaLight::~DiffuseAreaLight() noexcept
  {
  }

  real_t DiffuseAreaLight::pdfLi(const SurfaceInfo& ref, const Direction& wi) const
  {
    return _object->pdf(ref, wi);
  }

  Color DiffuseAreaLight::sampleLi(const SurfaceInfo& ref, Direction *wi,
                                   const Sample2D& xi, real_t *pdf, Ray *vis) const
  {
    const SurfaceInfo surface = _object->sample(ref, xi, pdf);
    *wi = geom::to_direction(n4::direction(ref.P, surface.P));
    if( vis != nullptr ) {
      *vis = ref.ray(*wi, TRACE_BIAS, n4::distance(ref.P, surface.P));
    }
    return radiance(surface, -(*wi));
  }

  Color DiffuseAreaLight::radiance(const SurfaceInfo& surface, const Direction& wo) const
  {
    return n4::dot(surface.N, geom::to_normal(wo)) > ZERO
        ? _Lemit
        : Color();
  }

  LightPtr DiffuseAreaLight::create(const IObject *object, const Color& Lemit)
  {
    return std::make_unique<DiffuseAreaLight>(object, Lemit);
  }

} // namespace rt
