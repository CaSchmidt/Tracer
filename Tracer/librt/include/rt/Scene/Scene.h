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

#ifndef SCENE_H
#define SCENE_H

#include "rt/Light/ILight.h"
#include "rt/Object/IObject.h"

namespace rt {

  class Scene {
  public:
    Scene();
    ~Scene();

    Scene(Scene&&);
    Scene& operator=(Scene&&);

    void add(LightPtr& light);
    void add(ObjectPtr& object);

    Color backgroundColor() const;
    void setBackgroundColor(const Color& color);

    void clear();

    bool intersect(SurfaceInfo *surface, const Ray& ray) const;
    bool intersect(const Ray& ray) const;

    const Lights& lights() const;

    bool useCastShadow() const;
    void setUseCastShadow(const bool on);

  private:
    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;

    Color _backgroundColor;
    Lights _lights;
    Objects _objects;
    bool _use_cast_shadow{false};
  };

} // namespace rt

#endif // SCENE_H
