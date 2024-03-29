/****************************************************************************
** Copyright (c) 2020, Carsten Schmidt. All rights reserved.
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

#include "rt/Object/Disk.h"

#include "geom/Intersect.h"
#include "rt/Object/SurfaceInfo.h"
#include "rt/Sampler/Sampling.h"

namespace rt {

  ////// Public //////////////////////////////////////////////////////////////

  bool isDisk(const ObjectPtr& o) // cf. IObject.h
  {
    return dynamic_cast<Disk*>(o.get()) != nullptr;
  }

  ////// public //////////////////////////////////////////////////////////////

  Disk::Disk(const Transform& objectToWorld,
             const real_t radius) noexcept
    : IObject(objectToWorld)
    , _radius{radius}
  {
  }

  Disk::~Disk() noexcept
  {
  }

  bool Disk::intersect(SurfaceInfo *surface, const Ray& ray) const
  {
    const Ray rayObj = toObject(ray);

    const real_t t = geom::intersect::plane(rayObj, EPSILON0);
    if( !rayObj.isValidT(t) ) {
      return false;
    }

    const Vertex Pobj = rayObj(t);
    const real_t    v = math::abs<real_t>(Pobj.x, Pobj.y)/_radius;
    if( v > ONE ) {
      return false;
    }

    if( surface != nullptr ) {
      const real_t u = math::phase<real_t>(Pobj.x, Pobj.y)/TWO_PI;

      *surface = SurfaceInfo();

      surface->object = this;
      surface->t      = t;
      surface->N      = toWorld(Normal{0, 0, 1});
      surface->P      = toWorld(Pobj);
      surface->u      = u;
      surface->v      = v;

      surface->initializeShading(ray);
    }

    return true;
  }

  real_t Disk::area() const
  {
    return PI*_radius*_radius;
  }

  SurfaceInfo Disk::sample(const Sample2D& xi, real_t *pdf) const
  {
    const Vertex Pdisk = ConcentricDisk::sample(xi);

    const Vertex Pobj{_radius*Pdisk.x, _radius*Pdisk.y, 0};
    const Normal Nobj{0, 0, 1};

    SurfaceInfo surface;
    surface.N = toWorld(Nobj);
    surface.P = toWorld(Pobj);

    if( pdf != nullptr ) {
      *pdf = ONE/area();
    }

    return surface;
  }

  ObjectPtr Disk::create(const Transform& objectToWorld,
                         const real_t radius)
  {
    if( radius <= ZERO ) {
      return ObjectPtr();
    }
    return std::make_unique<Disk>(objectToWorld, radius);
  }

} // namespace rt
