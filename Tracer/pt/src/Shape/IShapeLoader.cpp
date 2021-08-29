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

#include "pt/Shape/IShape.h"

#include "pt/Shape/Cylinder.h"
#include "pt/Shape/Disk.h"
#include "pt/Shape/Plane.h"
#include "pt/Shape/Sphere.h"
#include "rt/Loader/SceneLoaderStringUtil.h"

namespace pt {

  bool IShape::isShape(const tinyxml2::XMLElement *elem)
  {
    return elem != nullptr  &&  rt::priv::compare(elem->Value(), "Shape");
  }

  ShapePtr IShape::load(const tinyxml2::XMLElement *elem)
  {
    if( !isShape(elem) ) {
      return ShapePtr();
    }

    if(        Cylinder::isCylinder(elem) ) {
      return Cylinder::load(elem);
    } else if( Disk::isDisk(elem) ) {
      return Disk::load(elem);
    } else if( Plane::isPlane(elem) ) {
      return Plane::load(elem);
    } else if( Sphere::isSphere(elem) ) {
      return Sphere::load(elem);
    }

    return ShapePtr();
  }

} // namespace pt
