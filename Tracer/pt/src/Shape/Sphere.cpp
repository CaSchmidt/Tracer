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

#include "pt/Shape/Sphere.h"

#include "geom/Intersect.h"
#include "pt/Shape/IntersectionInfo.h"

namespace pt {

  ////// public //////////////////////////////////////////////////////////////

  Sphere::Sphere(const rt::Transform& objectToWorld,
                 const rt::real_t radius) noexcept
    : IShape(objectToWorld)
    , _radius{radius}
  {
  }

  Sphere::~Sphere() noexcept
  {
  }

  bool Sphere::intersect(IntersectionInfo *info, const rt::Ray& ray) const
  {
    const rt::Ray rayObj = toShape(ray);

    const rt::real_t t = geom::intersect::sphere(rayObj, _radius, EPSILON0);
    if( !rayObj.isValidT(t) ) {
      return false;
    }

    if( info != nullptr ) {
      const rt::Vertex Pobj = rayObj(t);
      const rt::Normal Nobj = geom::to_normal(n4::normalize(Pobj));
      const rt::real_t    u = math::phase<rt::real_t>(Nobj.x, Nobj.y)/rt::TWO_PI;
      const rt::real_t    v = n4::acos(std::clamp<rt::real_t>(Nobj.z, -rt::ONE, rt::ONE))/rt::PI;

      *info = IntersectionInfo();

      info->shape = this;
      info->t     = t;
      info->N     = toWorld(Nobj);
      info->P     = toWorld(Pobj);
      info->u     = u;
      info->v     = v;

      info->initializeShading(ray);
    }

    return true;
  }

  rt::Bounds Sphere::shapeBounds() const
  {
    return rt::Bounds(rt::Vertex(_radius), rt::Vertex(-_radius));
  }

  ShapePtr Sphere::create(const rt::Transform& objectToWorld,
                          const rt::real_t radius)
  {
    if( radius <= rt::ZERO ) {
      return ShapePtr();
    }
    return std::make_unique<Sphere>(objectToWorld, radius);
  }

} // namespace pt
