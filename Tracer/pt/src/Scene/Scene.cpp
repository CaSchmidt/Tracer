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

#include "pt/Scene/Scene.h"

#include "pt/Shape/IntersectionInfo.h"

namespace pt {

  ////// public //////////////////////////////////////////////////////////////

  Scene::Scene() noexcept
  {
    clear();
  }

  Scene::~Scene() noexcept
  {
  }

  void Scene::clear()
  {
    _background = rt::Color(0);
    _shapes.clear();
  }

  void Scene::add(ShapePtr& shape)
  {
    if( !shape ) {
      return;
    }
    _shapes.push_back(std::move(shape));
  }

  rt::Color Scene::backgroundColor() const
  {
    return _background;
  }

  void Scene::setBackgroundColor(const rt::Color& c)
  {
    _background = n4::clamp(c, 0, 1);
  }

  bool Scene::intersect(IntersectionInfo *info, const rt::Ray& ray) const
  {
    if( !ray.isValid() ) {
      return false;
    }

    *info = IntersectionInfo();
    for(const ShapePtr& shape : _shapes) {
      IntersectionInfo hit;
      if( !shape->intersect(&hit, ray) ) {
        continue;
      }
      if( !info->isHit()  ||  hit.t < info->t ) {
        *info = hit;
      }
    }

    return info->isHit();
  }

  bool Scene::intersect(const rt::Ray& ray) const
  {
    if( !ray.isValid() ) {
      return false;
    }

    for(const ShapePtr& shape : _shapes) {
      if( shape->intersect(nullptr, ray) ) {
        return true;
      }
    }

    return false;
  }

  rt::ScenePtr Scene::create()
  {
    return std::make_unique<Scene>();
  }

} // namespace pt
