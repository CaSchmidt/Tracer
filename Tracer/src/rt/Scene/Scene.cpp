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

#include "rt/Scene/Scene.h"

#include "rt/Scene/SurfaceInfo.h"

namespace rt {

  Scene::Scene() = default;

  Scene::~Scene() = default;

  Scene::Scene(Scene&&) = default;

  Scene& Scene::operator=(Scene&&) = default;

  void Scene::clear()
  {
    _lights.clear();
    _objects.clear();
  }

  void Scene::add(LightSourcePtr& light)
  {
    _lights.push_back(std::move(light));
  }

  void Scene::add(ObjectPtr& object)
  {
    _objects.push_back(std::move(object));
  }

  bool Scene::trace(SurfaceInfo& info, const Rayf& ray) const
  {
    info = SurfaceInfo();
    for(const ObjectPtr& object : _objects) {
      SurfaceInfo hit;
      if( !object->intersect(&hit, ray) ) {
        continue;
      }
      if( !info.isHit()  ||  hit.t < info.t ) {
        info = hit;
      }
    }
    return info.isHit();
  }

  bool Scene::trace(const Rayf& ray) const
  {
    for(const ObjectPtr& object : _objects) {
      if( object->material()->isShadowCaster()  &&  object->intersect(nullptr, ray) ) {
        return true;
      }
    }
    return false;
  }

  const LightSources& Scene::lights() const
  {
    return _lights;
  }

} // namespace rt
