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

#include "rt/Object/Cylinder.h"

#include "rt/Scene/SurfaceInfo.h"

namespace rt {

  namespace priv {

    void assign(SurfaceInfo& dest, const SurfaceInfo& src)
    {
      if( !geom::intersect::isHit(src.t) ) {
        return; // Nothing to do!
      }
      if( !geom::intersect::isHit(dest.t) ) {
        dest = src;
      } else if( src.t < dest.t ) {
        dest = src;
      }
    }

  } // namespace priv

  ////// public //////////////////////////////////////////////////////////////

  Cylinder::Cylinder(const Transformf& objectToWorld, MaterialPtr& material,
                     const real_T height, const real_T radius) noexcept
    : IObject(objectToWorld, material)
    , _height{height}
    , _radius{radius}
  {
  }

  Cylinder::~Cylinder() noexcept
  {
  }

  bool Cylinder::intersect(SurfaceInfo *info, const Rayf& ray) const
  {
    constexpr bool BOTTOM = true;
    constexpr bool    TOP = false;

    const Rayf rayObj = _xfrmOW*ray;

    if( info != nullptr ) {
      *info = SurfaceInfo();

      SurfaceInfo localInfo;
      if( intersectCylinder(&localInfo, rayObj) ) {
        priv::assign(*info, localInfo);
      }
      if( intersectDisc(&localInfo, rayObj, TOP) ) {
        priv::assign(*info, localInfo);
      }
      if( intersectDisc(&localInfo, rayObj, BOTTOM) ) {
        priv::assign(*info, localInfo);
      }

      info->object = this;

      return info->isHit();
    }

    return
        intersectCylinder(nullptr, rayObj)       ||
        intersectDisc(nullptr, rayObj, TOP)      ||
        intersectDisc(nullptr, rayObj, BOTTOM);
  }

  ObjectPtr Cylinder::create(const Transformf& objectToWorld, MaterialPtr& material,
                             const real_T height, const real_T radius)
  {
    return std::make_unique<Cylinder>(objectToWorld, material, height, radius);
  }

  ////// private /////////////////////////////////////////////////////////////

  bool Cylinder::intersectCylinder(SurfaceInfo *info, const Rayf& rayObj) const
  {
    const real_T t = geom::intersect::cylinder(rayObj, _radius);
    if( !geom::intersect::isHit(t) ) {
      return false;
    }

    const Vertex3f Pobj = rayObj(t);
    if( csAbs(Pobj.z) > _height/TWO ) {
      return false;
    }

    if( info != nullptr ) {
      const Normal3f Nobj = cs::normalize(Normal3f{Pobj.x, Pobj.y, 0});
      const real_T      u = math::phase<real_T>(Pobj.x, Pobj.y)/TWO_PI;
      const real_T      v = (Pobj.z + _height/2)/_height;

      *info = SurfaceInfo();

      info->t = t;
      info->N = _xfrmWO*Nobj;
      info->P = _xfrmWO*Pobj;
      info->u = u;
      info->v = v;
    }

    return false;
  }

  bool Cylinder::intersectDisc(SurfaceInfo *info, const Rayf& rayObj, const bool bottom) const
  {
    const real_T sign = bottom  ?  -ONE : ONE;

    const real_T t = geom::intersect::plane(rayObj, sign*_height/2);
    if( !geom::intersect::isHit(t) ) {
      return false;
    }

    const Vertex3f Pobj = rayObj(t);
    const real_T      u = math::phase<real_T>(Pobj.x, sign*Pobj.y)/TWO_PI;
    const real_T      v = math::abs<real_T>(Pobj.x, Pobj.y)/_radius;
    if( v > ONE ) {
      return false;
    }

    if( info != nullptr ) {
      *info = SurfaceInfo();

      info->t = t;
      info->N = sign*(_xfrmWO*Normal3f{0, 0, 1});
      info->P = _xfrmWO*Pobj;
      info->u = u;
      info->v = v;
    }

    return true;
  }

} // namespace rt
