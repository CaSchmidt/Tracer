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

#include "rt/Scene/Cylinder.h"

#include "geom/Intersect.h"

namespace rt {

  namespace priv {

    void assign(SurfaceInfo& dest, const SurfaceInfo& src)
    {
      if( !isHit(src.t) ) {
        return; // Nothing to do!
      }
      if( !isHit(dest.t) ) {
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

  bool Cylinder::intersect(SurfaceInfo& info, const Rayf& ray) const
  {
    info = SurfaceInfo();

    const Rayf rayObj = _xfrmOW*ray;

    priv::assign(info, intersectCylinder(rayObj));
    priv::assign(info, intersectDisc(rayObj, false));
    priv::assign(info, intersectDisc(rayObj, true));
    if( !isHit(info.t) ) {
      return info.isHit();
    }

    info.object = this;

    return info.isHit();
  }

  ObjectPtr Cylinder::create(const Transformf& objectToWorld, MaterialPtr& material,
                             const real_T height, const real_T radius)
  {
    return std::make_unique<Cylinder>(objectToWorld, material, height, radius);
  }

  ////// private /////////////////////////////////////////////////////////////

  SurfaceInfo Cylinder::intersectCylinder(const Rayf& rayObj) const
  {
    SurfaceInfo info;

    info.t = geom::intersect::cylinder(rayObj, _radius);
    if( !isHit(info.t) ) {
      return SurfaceInfo();
    }

    const Vertex3f Pobj = rayObj(info.t);
    if( std::abs(Pobj.z) > _height/2 ) {
      return SurfaceInfo();
    }

    const Normal3f Nobj = cs::normalize(Normal3f{Pobj.x, Pobj.y, 0});
    const real_T      u = math::phase<real_T>(Pobj.x, Pobj.y)/TWO_PI;
    const real_T      v = (Pobj.z + _height/2)/_height;

    info.N = _xfrmWO*Nobj;
    info.P = _xfrmWO*Pobj;
    info.u = u;
    info.v = v;

    return info;
  }

  SurfaceInfo Cylinder::intersectDisc(const Rayf& rayObj, const bool bottom) const
  {
    SurfaceInfo info;

    const real_T sign = bottom  ?  -ONE : ONE;

    info.t = geom::intersect::plane(rayObj, sign*_height/2);
    if( !isHit(info.t) ) {
      return SurfaceInfo();
    }

    const Vertex3f Pobj = rayObj(info.t);
    const real_T      u = math::phase<real_T>(Pobj.x, sign*Pobj.y)/TWO_PI;
    const real_T      v = math::abs<real_T>(Pobj.x, Pobj.y)/_radius;
    if( v > 1 ) {
      return SurfaceInfo();
    }

    info.N = sign*(_xfrmWO*geom::zAxis<Normal3f>());
    info.P = _xfrmWO*Pobj;
    info.u = u;
    info.v = v;

    return info;
  }

} // namespace rt
