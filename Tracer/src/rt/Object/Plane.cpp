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

#include "rt/Scene/SurfaceInfo.h"

namespace rt {

  namespace priv {

    constexpr bool isBounding(const real_T& value)
    {
      return ZERO <= value  &&  value <= ONE;
    }

    constexpr real_T normalized(const real_T& value, const real_T& lower, const real_T& scale)
    {
      return (value - lower)/scale;
    }

  } // namespace priv

  ////// public //////////////////////////////////////////////////////////////

  Plane::Plane(const Transformf& objectToWorld, MaterialPtr& material,
               const real_T width, const real_T height) noexcept
    : IObject(objectToWorld, material)
    , _width{width}
    , _height{height}
  {
  }

  Plane::~Plane() noexcept
  {
  }

  bool Plane::intersect(SurfaceInfo *info, const Rayf& ray) const
  {
    const Rayf rayObj = _xfrmOW*ray;

    const real_T t = geom::intersect::plane(rayObj);
    if( !geom::intersect::isHit(t) ) {
      return false;
    }

    const Vertex3f Pobj = rayObj(t);
    const real_T      u = priv::normalized(Pobj.x, -_width/2,  _width);
    const real_T      v = priv::normalized(Pobj.y, -_height/2, _height);
    if( !priv::isBounding(u)  ||  !priv::isBounding(v) ) {
      return false;
    }

    if( info != nullptr ) {
      *info = SurfaceInfo();

      info->object = this;
      info->t      = t;
      info->N      = _xfrmWO*Normal3f{0, 0, 1};
      info->P      = _xfrmWO*Pobj;
      info->u      = u;
      info->v      = v;
    }

    return true;
  }

  ObjectPtr Plane::create(const Transformf& objectToWorld, MaterialPtr& material,
                          const real_T width, const real_T height)
  {
    return std::make_unique<Plane>(objectToWorld, material, width, height);
  }

} // namespace rt
