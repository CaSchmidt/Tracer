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

#include "rt/Object/IObject.h"

#include "rt/Object/SurfaceInfo.h"

namespace rt {

  ////// public //////////////////////////////////////////////////////////////

  IObject::IObject(const Transform& objectToWorld) noexcept
  {
    setObjectToWorld(objectToWorld);
  }

  IObject::~IObject() noexcept
  {
  }

  bool IObject::castShadow(const Ray& ray) const
  {
    const bool is_shadow_caster = _material
        ? _material->isShadowCaster()
        : true;
    return is_shadow_caster  &&  intersect(nullptr, ray);
  }

  IAreaLight *IObject::areaLight()
  {
    return _areaLight;
  }

  const IAreaLight *IObject::areaLight() const
  {
    return _areaLight;
  }

  void IObject::setAreaLight(IAreaLight *light)
  {
    _areaLight = light;
  }

  IMaterial *IObject::material()
  {
    return _material.get();
  }

  const IMaterial *IObject::material() const
  {
    return _material.get();
  }

  void IObject::setMaterial(MaterialPtr& material)
  {
    _material = std::move(material);
  }

  void IObject::setMaterial(MaterialPtr&& material)
  {
    _material = std::move(material);
  }

  void IObject::moveObject(const Transform& objectToWorld)
  {
    _xfrmWO = objectToWorld*_xfrmWO;
    _xfrmOW = _xfrmWO.inverse();
  }

  const Transform& IObject::objectToWorld() const
  {
    return _xfrmWO;
  }

  void IObject::setObjectToWorld(const Transform& objectToWorld)
  {
    _xfrmWO = Transform();
    moveObject(objectToWorld);
  }

  real_t IObject::pdf(const SurfaceInfo& /*surface*/) const
  {
    return ONE/area();
  }

  SurfaceInfo IObject::sample(const SurfaceInfo& ref, const Sample2D& xi, real_t *pdf) const
  {
    const SurfaceInfo surface = sample(xi, pdf);
    const Vertex        delta = surface.P - ref.P;     // NOTE: direction := to - from
    const real_t           rr = n4::dot(delta, delta); // Squared Distance

    if( pdf == nullptr ) {
      return surface;
    }

    if( rr != ZERO ) {
      // NOTE: 'wi' points towards 'surface'; cf. 'delta'!
      const Direction    wi = geom::to_direction(delta)/Math::sqrt(rr);
      const real_t absCosTo = geom::absDot(surface.N, -wi);
      if( absCosTo != ZERO ) {
        *pdf *= rr/absCosTo; // NOTE: Convert from area to solid angle.
      } else {
        *pdf = 0;
      }
    } else {
      *pdf = 0;
    }

    return surface;
  }

  real_t IObject::pdf(const SurfaceInfo& ref, const Direction& wi) const
  {
    const Ray ray = ref.ray(wi);
    SurfaceInfo surface;
    if( !intersect(&surface, ray) ) {
      return 0;
    }

    const Vertex    delta = surface.P - ref.P;
    const real_t       rr = n4::dot(delta, delta); // Squared Distance
    const real_t absCosTo = geom::absDot(surface.N, -wi);
    if( absCosTo == ZERO ) {
      return 0;
    }

    return rr/absCosTo/area(); // NOTE: Return solid angle.
  }

} // namespace rt
