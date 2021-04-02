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

#include "rt/Object/Sphere.h"

#include "geom/Intersect.h"
#include "rt/Object/SurfaceInfo.h"

namespace rt {

  Sphere::Sphere(const Transform& objectToWorld,
                 const real_t radius) noexcept
    : IObject(objectToWorld)
    , _radius{radius}
  {
  }

  Sphere::~Sphere() noexcept
  {
  }

  bool Sphere::intersect(SurfaceInfo *surface, const Ray& ray) const
  {
    const Ray rayObj = toObject(ray);

    const real_t t = geom::intersect::sphere(rayObj, _radius);
    if( !rayObj.isValid(t) ) {
      return false;
    }

    if( surface != nullptr ) {
      const Vertex Pobj = rayObj(t);
      const Normal Nobj = geom::to_normal(n4::normalize(Pobj));
      const real_t    u = math::phase<real_t>(Nobj.x, Nobj.y)/TWO_PI;
      const real_t    v = n4::acos(std::clamp<real_t>(Nobj.z, -ONE, ONE))/PI;

      *surface = SurfaceInfo();

      surface->object = this;
      surface->t      = t;
      surface->wo     = -ray.direction();
      surface->N      = toWorld(Nobj);
      surface->P      = toWorld(Pobj);
      surface->u      = u;
      surface->v      = v;
    }

    return true;
  }

  ObjectPtr Sphere::create(const Transform& objectToWorld,
                           const real_t radius)
  {
    return std::make_unique<Sphere>(objectToWorld, radius);
  }

} // namespace rt
