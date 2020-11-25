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
#include "rt/Scene/SurfaceInfo.h"

namespace rt {

  Sphere::Sphere(const Transformf& objectToWorld, MaterialPtr& material,
                 const real_T radius) noexcept
    : IObject(objectToWorld, material)
    , _radius{radius}
  {
  }

  Sphere::~Sphere() noexcept
  {
  }

  bool Sphere::intersect(SurfaceInfo *info, const Rayf& ray) const
  {
    const Rayf rayObj = _xfrmOW*ray;

    const real_T t = geom::intersect::sphere(rayObj, _radius);
    if( !rayObj.isValid(t) ) {
      return false;
    }

    if( info != nullptr ) {
      const Vertex3f Pobj = rayObj(t);
      const Normal3f Nobj = to_normal(cs::normalize(Pobj));
      const real_T      u = math::phase<real_T>(Nobj.x, Nobj.y)/TWO_PI;
      const real_T      v = csACos(csClamp(Nobj.z, -ONE, ONE))/PI;

      *info = SurfaceInfo();

      info->object = this;
      info->t      = t;
      info->N      = _xfrmWO*Nobj;
      info->P      = _xfrmWO*Pobj;
      info->u      = u;
      info->v      = v;
    }

    return true;
  }

  ObjectPtr Sphere::create(const Transformf& objectToWorld, MaterialPtr& material,
                           const real_T radius)
  {
    return std::make_unique<Sphere>(objectToWorld, material, radius);
  }

} // namespace rt
