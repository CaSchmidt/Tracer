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

#include "pt/BSDF/IBSDF.h"

#include "pt/BSDF/Dielectric.h"
#include "pt/BSDF/Diffuse.h"
#include "pt/BSDF/Mirror.h"
#include "rt/Loader/SceneLoaderBase.h"
#include "rt/Loader/SceneLoaderStringUtil.h"

namespace pt {

  ////// public //////////////////////////////////////////////////////////////

  bool IBSDF::isBSDF(const tinyxml2::XMLElement *elem)
  {
    return elem != nullptr  &&  rt::priv::compare(elem->Value(), "BSDF");
  }

  BSDFPtr IBSDF::load(const tinyxml2::XMLElement *elem)
  {
    if( !isBSDF(elem) ) {
      return BSDFPtr();
    }

    BSDFPtr bsdf;
    if(        Dielectric::isDielectric(elem) ) {
      bsdf = Dielectric::load(elem);
    } else if( Diffuse::isDiffuse(elem) ) {
      bsdf = Diffuse::load(elem);
    } else if( Mirror::isMirror(elem) ) {
      bsdf = Mirror::load(elem);
    }

    if( bsdf ) {
      bsdf->setColor(readColor(elem));
    }

    return bsdf;
  }

  ////// private /////////////////////////////////////////////////////////////

  rt::Color IBSDF::readColor(const tinyxml2::XMLElement *parent)
  {
    const rt::Color defaultColor(1);

    if( parent == nullptr ) {
      return defaultColor;
    }

    bool ok = false;

    const rt::Color result = rt::priv::parseColor(parent->FirstChildElement("BSDF.Color"), &ok);
    if( !ok ) {
      return defaultColor;
    }

    return result;
  }

} // namespace pt
