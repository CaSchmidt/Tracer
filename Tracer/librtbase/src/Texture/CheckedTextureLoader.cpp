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

#include "rt/Texture/CheckedTexture.h"

#include "rt/Loader/SceneLoaderBase.h"

namespace rt {

  TexturePtr CheckedTexture::load(const tinyxml2::XMLElement *elem)
  {
    if( elem == nullptr ) {
      return TexturePtr();
    }

    bool myOk = false;

    const size_t id = readId(elem);

    const Color colorA = priv::parseColor(elem->FirstChildElement("ColorA"), &myOk);
    if( !myOk ) {
      return TexturePtr();
    }

    const Color colorB = priv::parseColor(elem->FirstChildElement("ColorB"), &myOk);
    if( !myOk ) {
      return TexturePtr();
    }

    const real_t scaleS = priv::parseReal(elem->FirstChildElement("ScaleS"), &myOk);
    if( !myOk ) {
      return TexturePtr();
    }

    const real_t scaleT = priv::parseReal(elem->FirstChildElement("ScaleT"), &myOk);
    if( !myOk ) {
      return TexturePtr();
    }

    return CheckedTexture::create(id, colorA, colorB, scaleS, scaleT);
  }

} // namespace rt
