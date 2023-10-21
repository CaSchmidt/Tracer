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

#include "pt/Shape/Cylinder.h"

#include "geom/Intersect.h"
#include "pt/Shape/IntersectionInfo.h"

namespace pt {

  ////// public //////////////////////////////////////////////////////////////

  Cylinder::Cylinder(const rt::Transform& objectToWorld,
                     const rt::real_t height, const rt::real_t radius) noexcept
    : IShape(objectToWorld)
    , _height{height}
    , _radius{radius}
  {
  }

  Cylinder::~Cylinder() noexcept
  {
  }

  bool Cylinder::intersect(IntersectionInfo *info, const rt::Ray& ray) const
  {
    const rt::Ray rayObj = toShape(ray);

    const rt::real_t t = geom::intersect::cylinder(rayObj, _radius, EPSILON0);
    if( !rayObj.isValidT(t) ) {
      return false;
    }

    const rt::Vertex Pobj = rayObj(t);
    if( rt::Math::abs(Pobj.z) > _height/rt::TWO ) {
      return false;
    }

    if( info != nullptr ) {
      const rt::Normal Nobj = n4::normalize(rt::Normal{Pobj.x, Pobj.y, 0});
      const rt::real_t    u = math::phase<rt::real_t>(Pobj.x, Pobj.y)/rt::TWO_PI;
      const rt::real_t    v = (Pobj.z + _height/2)/_height;

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

  rt::Bounds Cylinder::shapeBounds() const
  {
    const rt::real_t rz = _height/rt::TWO;
    return rt::Bounds(rt::Vertex{_radius, _radius, rz}, rt::Vertex{-_radius, -_radius, -rz});
  }

  ShapePtr Cylinder::create(const rt::Transform& objectToWorld,
                            const rt::real_t height, const rt::real_t radius)
  {
    if( height <= rt::ZERO  ||  radius <= rt::ZERO ) {
      return ShapePtr();
    }
    return std::make_unique<Cylinder>(objectToWorld, height, radius);
  }

} // namespace pt
