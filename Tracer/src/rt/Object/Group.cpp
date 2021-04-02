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

#include "rt/Object/Group.h"

#include "rt/Object/SurfaceInfo.h"

namespace rt {

  ////// public //////////////////////////////////////////////////////////////

  Group::Group(const Transform& objectToWorld) noexcept
    : IObject(objectToWorld)
    , _objects{}
  {
  }

  Group::~Group() noexcept
  {
  }

  void Group::add(ObjectPtr& object)
  {
    if( object ) {
      _objects.push_back(std::move(object));
    }
  }

  void Group::clear()
  {
    _objects.clear();
  }

  bool Group::castShadow(const Ray &ray) const
  {
    const Ray rayObj = toObject(ray);
    for(const ObjectPtr& o : _objects) {
      if( o->castShadow(rayObj) ) {
        return true;
      }
    }
    return false;
  }

  bool Group::intersect(SurfaceInfo *surface, const Ray& ray) const
  {
    const Ray rayObj = toObject(ray);

    if( surface != nullptr ) {
      *surface = SurfaceInfo();
      for(const ObjectPtr& o : _objects) {
        SurfaceInfo hit;
        if( o->intersect(&hit, rayObj) ) {
          if( !surface->isHit()  ||  hit.t < surface->t ) {
            *surface = hit;
          }
        }
      }
      if( surface->isHit() ) {
        surface->wo = toWorld(surface->wo);
        surface->N  = toWorld(surface->N);
        surface->P  = toWorld(surface->P);
        return true;
      }

    } else {
      for(const ObjectPtr& o : _objects) {
        if( o->intersect(nullptr, rayObj) ) {
          return true;
        }
      }
    }

    return false;
  }

  ObjectPtr Group::create(const Transform& objectToWorld)
  {
    return std::make_unique<Group>(objectToWorld);
  }

} // namespace rt
