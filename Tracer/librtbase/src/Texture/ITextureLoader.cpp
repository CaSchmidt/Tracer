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

#include <charconv>

#include <tinyxml2.h>

#include "rt/Texture/ITexture.h"

#include "rt/Loader/SceneLoaderStringUtil.h"
#include "rt/Texture/CheckedTexture.h"
#include "rt/Texture/FlatTexture.h"

namespace rt {

  ////// public //////////////////////////////////////////////////////////////

  bool ITexture::isTexture(const tinyxml2::XMLElement *elem)
  {
    return elem != nullptr  &&  priv::compare(elem->Value(), "Texture");
  }

  TexturePtr ITexture::load(const tinyxml2::XMLElement *elem)
  {
    /*
     * NOTE:
     * Some legacy applications require elem's name to differ from isTexture(),
     * so calling load() on proper XML elements is left to the caller's discretion!
     */
    if( elem == nullptr ) {
      return TexturePtr();
    }

    if(        elem->Attribute("type", "Checked") != nullptr ) {
      return CheckedTexture::load(elem);
    } else if( elem->Attribute("type", "Flat") != nullptr ) {
      return FlatTexture::load(elem);
    }

    return TexturePtr();
  }

  ////// protected ///////////////////////////////////////////////////////////

  size_t ITexture::readId(const tinyxml2::XMLElement *elem)
  {
    constexpr size_t defaultId = 0; // Default ID
    const char *nameId = "id";

    if( elem == nullptr  ||  elem->Attribute(nameId) == nullptr ) {
      return defaultId;
    }

    size_t value;
    const char *attr = elem->Attribute(nameId);
    const std::from_chars_result result =
        std::from_chars(attr, attr + priv::length(attr), value, 10);
    if( result.ec != std::errc() ) {
      return defaultId;
    }

    return value;
  }

} // namespace rt