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

#include "pt/Scene/Object.h"

#include "pt/Shape/IntersectionInfo.h"

namespace pt {

  ////// public //////////////////////////////////////////////////////////////

  Object::Object(const rt::Transform& objectToWorld) noexcept
    : _xformWO(objectToWorld)
  {
  }

  Object::~Object() noexcept
  {
  }

  void Object::add(ShapePtr& shape)
  {
    if( !shape ) {
      return;
    }
    _faces.push_back(ObjectFace(shape));
    _faces.back().shape->moveShape(_xformWO);
  }

  bool Object::intersect(IntersectionInfo *info, const rt::Ray& ray) const
  {
    if( !ray.isValid() ) {
      return false;
    }

    if( !_bounds.isValid()  ||  !_bounds.intersect(ray) ) {
      return false;
    }

    *info = IntersectionInfo();

    for(const ObjectFace& face : _faces) {
      IntersectionInfo hit;
      if( !face.shape->intersect(&hit, ray) ) {
        continue;
      } else {
        hit.object  = this;
        hit.texture = face.texture.get();
      }
      if( !info->isHit()  ||  hit.t < info->t ) {
        *info = hit;
      }
    }

    if( info->isHit()  &&  info->texture == nullptr ) {
      info->texture = _texture.get();
    }

    return info->isHit();
  }

  rt::Color Object::emittance() const
  {
    return _emitColor*_emitScale;
  }

  void Object::setEmissiveColor(const rt::Color& c)
  {
    _emitColor = n4::clamp(c, 0, 1);
  }

  void Object::setEmissiveScale(const rt::real_t s)
  {
    _emitScale = std::max<rt::real_t>(1, s);
  }

  bool Object::haveBSDF() const
  {
    return bool(_bsdf);
  }

  const IBSDF *Object::bsdf() const
  {
    return _bsdf.get();
  }

  bool Object::setBSDF(BSDFPtr& bsdf)
  {
    _bsdf = std::move(bsdf);
    return haveBSDF();
  }

  bool Object::haveTexture() const
  {
    return bool(_texture);
  }

  bool Object::setTexture(rt::TexturePtr& texture)
  {
    if( !texture ) {
      return false;
    }
    return setTexture(texture->id(), texture);
  }

  bool Object::setTexture(const rt::size_t id, rt::TexturePtr& texture)
  {
    if( !texture ) {
      return false;
    }
    if( id == 0 ) {
      _texture = std::move(texture);
      return haveTexture();
    }
    if( id < 1  ||  id > _faces.size() ) {
      return false;
    }
    ObjectFaces::iterator face = std::next(_faces.begin(), id - 1);
    face->texture = std::move(texture);
    return bool(face->texture);
  }

  void Object::preprocess()
  {
    _bounds = rt::Bounds();
    for(const ObjectFace& face : _faces) {
      const rt::Bounds wb = face.shape->worldBounds();
      _bounds.update(wb.min());
      _bounds.update(wb.max());
    }
  }

  ////// public static ///////////////////////////////////////////////////////

  ObjectPtr Object::create(const rt::Transform& objectToWorld)
  {
    return std::make_unique<Object>(objectToWorld);
  }

} // namespace pt
