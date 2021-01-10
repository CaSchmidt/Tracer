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

#include "rt/Loader/SceneLoaderBase.h"
#include "rt/Texture/CheckedTexture.h"
#include "rt/Texture/FlatTexture.h"

namespace rt {

  namespace priv {

    // Implementation ////////////////////////////////////////////////////////

    TexturePtr parseCheckedTexture(const tinyxml2::XMLElement *node)
    {
      bool myOk = false;

      const Color colorA = parseColor(node->FirstChildElement("ColorA"), &myOk);
      if( !myOk ) {
        return TexturePtr();
      }

      const Color colorB = parseColor(node->FirstChildElement("ColorB"), &myOk);
      if( !myOk ) {
        return TexturePtr();
      }

      const real_t scaleS = parseReal(node->FirstChildElement("ScaleS"), &myOk);
      if( !myOk  ||  scaleS <= 0 ) {
        return TexturePtr();
      }

      const real_t scaleT = parseReal(node->FirstChildElement("ScaleT"), &myOk);
      if( !myOk  ||  scaleT <= 0 ) {
        return TexturePtr();
      }

      return CheckedTexture::create(colorA, colorB, scaleS, scaleT);
    }

    TexturePtr parseFlatTexture(const tinyxml2::XMLElement *node)
    {
      bool myOk = false;

      const Color color = parseColor(node->FirstChildElement("Color"), &myOk);
      if( !myOk ) {
        return TexturePtr();
      }

      return FlatTexture::create(color);
    }

    // Export ////////////////////////////////////////////////////////////////

    TexturePtr parseTexture(const tinyxml2::XMLElement *node)
    {
      if( node == nullptr ) {
        return TexturePtr();
      }

      if(        node->Attribute("type", "Checked") != nullptr ) {
        return parseCheckedTexture(node);
      } else if( node->Attribute("type", "Flat") != nullptr ) {
        return parseFlatTexture(node);
      }

      return TexturePtr();
    }

  } // namespace priv

} // namespace rt
