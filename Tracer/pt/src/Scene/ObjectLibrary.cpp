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

#include "pt/Shape/Plane.h"

namespace pt {

  ObjectPtr Object::createBox(const rt::Transform& objectToWorld,
                              const rt::real_t dimx,
                              const rt::real_t dimy,
                              const rt::real_t dimz)
  {
    if( dimx <= rt::ZERO  ||  dimy <= rt::ZERO  ||  dimz <= rt::ZERO ) {
      return ObjectPtr();
    }

    ObjectPtr object = create(objectToWorld);
    ShapePtr shape;
    rt::Matrix X;

    // (X.1) Left ////////////////////////////////////////////////////////////

    const rt::real_t left = -dimx/rt::TWO;
    X = n4::translate(left, 0, 0)*n4::rotateYbyPI2(3);
    shape = Plane::create(X, dimz, dimy);
    object->add(shape);

    // (X.2) Right ///////////////////////////////////////////////////////////

    const rt::real_t right = dimx/rt::TWO;
    X = n4::translate(right, 0, 0)*n4::rotateYbyPI2(1);
    shape = Plane::create(X, dimz, dimy);
    object->add(shape);

    // (Y.3) Front ///////////////////////////////////////////////////////////

    const rt::real_t front = -dimy/rt::TWO;
    X = n4::translate(0, front, 0)*n4::rotateXbyPI2(1);
    shape = Plane::create(X, dimx, dimz);
    object->add(shape);

    // (Y.4) Back ////////////////////////////////////////////////////////////

    const rt::real_t back = dimy/rt::TWO;
    X = n4::translate(0, back, 0)*n4::rotateXbyPI2(3);
    shape = Plane::create(X, dimx, dimz);
    object->add(shape);

    // (Z.5) Bottom //////////////////////////////////////////////////////////

    const rt::real_t bottom = -dimz/rt::TWO;
    X = n4::translate(0, 0, bottom)*n4::rotateXbyPI2(2);
    shape = Plane::create(X, dimx, dimy);
    object->add(shape);

    // (Z.6) Top /////////////////////////////////////////////////////////////

    const rt::real_t top = dimz/rt::TWO;
    X = n4::translate(0, 0, top);
    shape = Plane::create(X, dimx, dimy);
    object->add(shape);

    return object;
  }

  ObjectPtr Object::createInvertedBox(const rt::Transform& objectToWorld,
                                      const rt::real_t dimx,
                                      const rt::real_t dimy,
                                      const rt::real_t dimz)
  {
    if( dimx <= rt::ZERO  ||  dimy <= rt::ZERO  ||  dimz <= rt::ZERO ) {
      return ObjectPtr();
    }

    ObjectPtr object = create(objectToWorld);
    ShapePtr shape;
    rt::Matrix X;

    // (X.1) Left ////////////////////////////////////////////////////////////

    const rt::real_t left = -dimx/rt::TWO;
    X = n4::translate(left, 0, 0)*n4::rotateYbyPI2(1);
    shape = Plane::create(X, dimz, dimy);
    object->add(shape);

    // (X.2) Right ///////////////////////////////////////////////////////////

    const rt::real_t right = dimx/rt::TWO;
    X = n4::translate(right, 0, 0)*n4::rotateYbyPI2(3);
    shape = Plane::create(X, dimz, dimy);
    object->add(shape);

    // (Y.3) Front ///////////////////////////////////////////////////////////

    const rt::real_t front = -dimy/rt::TWO;
    X = n4::translate(0, front, 0)*n4::rotateXbyPI2(3);
    shape = Plane::create(X, dimx, dimz);
    object->add(shape);

    // (Y.4) Back ////////////////////////////////////////////////////////////

    const rt::real_t back = dimy/rt::TWO;
    X = n4::translate(0, back, 0)*n4::rotateXbyPI2(1);
    shape = Plane::create(X, dimx, dimz);
    object->add(shape);

    // (Z.5) Bottom //////////////////////////////////////////////////////////

    const rt::real_t bottom = -dimz/rt::TWO;
    X = n4::translate(0, 0, bottom);
    shape = Plane::create(X, dimx, dimy);
    object->add(shape);

    // (Z.6) Top /////////////////////////////////////////////////////////////

    const rt::real_t top = dimz/rt::TWO;
    X = n4::translate(0, 0, top)*n4::rotateXbyPI2(2);
    shape = Plane::create(X, dimx, dimy);
    object->add(shape);

    return object;
  }

} // namespace pt
