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

#include <tinyxml2.h>

#include "pt/Shape/Cylinder.h"

#include "rt/Loader/SceneLoaderBase.h"

namespace pt {

  bool Cylinder::isCylinder(const tinyxml2::XMLElement *elem)
  {
    return IShape::isShape(elem)  &&  elem->Attribute("type", "Cylinder") != nullptr;
  }

  ShapePtr Cylinder::load(const tinyxml2::XMLElement* elem)
  {
    if( !isCylinder(elem) ) {
      return ShapePtr();
    }

    bool ok = false;

    const rt::real_t height = rt::priv::parseReal(elem->FirstChildElement("Height"), &ok);
    if( !ok ) {
      return ShapePtr();
    }

    const rt::real_t radius = rt::priv::parseReal(elem->FirstChildElement("Radius"), &ok);
    if( !ok ) {
      return ShapePtr();
    }

    rt::Transform transform = rt::priv::parseTransform(elem->FirstChildElement("Transform"), &ok);
    if( !ok ) {
      return ShapePtr();
    }

    return create(transform, height, radius);
  }

} // namespace pt
