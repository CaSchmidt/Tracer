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

#ifndef OBJECT_H
#define OBJECT_H

#include "pt/Shape/IShape.h"
#include "rt/Texture/ITexture.h"

namespace pt {

  using ObjectPtr = std::unique_ptr<class Object>;

  class Object {
  public:
    Object(const rt::Transform& objectToWorld) noexcept;
    ~Object() noexcept;

    void add(ShapePtr& shape);

    bool intersect(IntersectionInfo *info, const rt::Ray& ray) const;

    rt::Color emittance() const;
    void setEmissiveColor(const rt::Color& c);
    void setEmissiveScale(const rt::real_t s);

    bool setTexture(const rt::size_t id, rt::TexturePtr& texture);

    void preprocess();

    static ObjectPtr create(const rt::Transform& objectToWorld);

    static ObjectPtr createBox(const rt::Transform& objectToWorld,
                               const rt::real_t dimx,
                               const rt::real_t dimy,
                               const rt::real_t dimz);

    static ObjectPtr createInvertedBox(const rt::Transform& objectToWorld,
                                       const rt::real_t dimx,
                                       const rt::real_t dimy,
                                       const rt::real_t dimz);

  private:
    struct ObjectFace {
      ObjectFace(ShapePtr& _shape) noexcept
        : shape(std::move(_shape))
      {
      }

      ShapePtr       shape;
      rt::TexturePtr texture;

    private:
      ObjectFace() noexcept = delete;
    };

    using ObjectFaces = std::list<ObjectFace>;

    Object() noexcept = delete;

    rt::Bounds     _bounds;
    rt::Color      _emitColor{0, 0, 0};
    rt::real_t     _emitScale{1};
    ObjectFaces    _faces;
    rt::TexturePtr _texture;
    rt::Transform  _xformWO{}; // Object -> World
  };

  using Objects = std::list<ObjectPtr>;

} // namespace pt

#endif // OBJECT_H
