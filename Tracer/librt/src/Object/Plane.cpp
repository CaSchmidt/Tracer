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

#include "rt/Object/Plane.h"

#include "geom/Intersect.h"
#include "rt/Object/SurfaceInfo.h"

namespace rt {

  namespace priv {

    constexpr bool isBounding(const real_t& value)
    {
      return ZERO <= value  &&  value <= ONE;
    }

    constexpr real_t normalized(const real_t& value, const real_t& lower, const real_t& scale)
    {
      return (value - lower)/scale;
    }

  } // namespace priv

  ////// Public //////////////////////////////////////////////////////////////

  bool isPlane(const ObjectPtr& o) // cf. IObject.h
  {
    return dynamic_cast<Plane*>(o.get()) != nullptr;
  }

  ////// public //////////////////////////////////////////////////////////////

  Plane::Plane(const Transform& objectToWorld,
               const real_t width, const real_t height) noexcept
    : IObject(objectToWorld)
    , _width{width}
    , _height{height}
  {
  }

  Plane::~Plane() noexcept
  {
  }

  bool Plane::intersect(SurfaceInfo *surface, const Ray& ray) const
  {
    const Ray rayObj = toObject(ray);

    const real_t t = geom::intersect::plane(rayObj, EPSILON0);
    if( !rayObj.isValidT(t) ) {
      return false;
    }

    const Vertex Pobj = rayObj(t);
    const real_t    u = priv::normalized(Pobj.x, -_width/2,  _width);
    const real_t    v = priv::normalized(Pobj.y, -_height/2, _height);
    if( !priv::isBounding(u)  ||  !priv::isBounding(v) ) {
      return false;
    }

    if( surface != nullptr ) {
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

  real_t Plane::area() const
  {
    return _width*_height;
  }

  SurfaceInfo Plane::sample(const Sample2D& xi, real_t *pdf) const
  {
    SAMPLES_2D(xi);
    const real_t x = xi1*_width  - _width/2;
    const real_t y = xi2*_height - _height/2;

    const Vertex Pobj{x, y, 0};
    const Normal Nobj{0, 0, 1};

    SurfaceInfo surface;
    surface.N = toWorld(Nobj);
    surface.P = toWorld(Pobj);

    if( pdf != nullptr ) {
      *pdf = ONE/area();
    }

    return surface;
  }

  ObjectPtr Plane::create(const Transform& objectToWorld,
                          const real_t width, const real_t height)
  {
    if( width <= ZERO  ||  height <= ZERO ) {
      return ObjectPtr();
    }
    return std::make_unique<Plane>(objectToWorld, width, height);
  }

} // namespace rt
