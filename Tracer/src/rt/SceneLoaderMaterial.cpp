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

#include "priv/rt/SceneLoaderBase.h"
#include "rt/Material/MirrorMaterial.h"
#include "rt/Material/OpaqueMaterial.h"
#include "rt/Material/TransparentMaterial.h"
#include "rt/Texture/ITexture.h"

namespace rt {

  namespace priv {

    // Import ////////////////////////////////////////////////////////////////

    TexturePtr parseTexture(const tinyxml2::XMLElement *node);

    // Implementation ////////////////////////////////////////////////////////

    MaterialPtr parseMirrorMaterial(const tinyxml2::XMLElement *node)
    {
      bool myOk = false;

      const real_T reflectance = parseNodeAsFloat<real_T>(node->FirstChildElement("Reflectance"), &myOk);
      if( !myOk  ||  reflectance <= 0 ) {
        return MaterialPtr();
      }

      MaterialPtr result = MirrorMaterial::create();
      result->mirror()->setReflectance(reflectance);

      return result;
    }

    MaterialPtr parseOpaqueMaterial(const tinyxml2::XMLElement *node)
    {
      bool myOk = false;

      TexturePtr diffuse = parseTexture(node->FirstChildElement("Diffuse"));
      if( !diffuse ) {
        return MaterialPtr();
      }

      real_T shininess = 0;
      if( node->FirstChildElement("Shininess") != nullptr ) {
        shininess = parseNodeAsFloat<real_T>(node->FirstChildElement("Shininess"), &myOk);
        if( !myOk  ||  shininess < 1 ) {
          return MaterialPtr();
        }
      }

      TexturePtr specular;
      if( node->FirstChildElement("Specular") != nullptr ) {
        specular = parseTexture(node->FirstChildElement("Specular"));
        if( !specular ) {
          return MaterialPtr();
        }
      }

      MaterialPtr result = OpaqueMaterial::create();
      result->opaque()->setDiffuse(diffuse);
      result->opaque()->setShininess(shininess);
      result->opaque()->setSpecular(specular);

      return result;
    }

    MaterialPtr parseTransparentMaterial(const tinyxml2::XMLElement *node)
    {
      bool myOk = false;

      const real_T refraction = parseNodeAsFloat<real_T>(node->FirstChildElement("Refraction"), &myOk);
      if( !myOk  ||  refraction < 1 ) {
        return MaterialPtr();
      }

      MaterialPtr result = TransparentMaterial::create();
      result->transparent()->setRefraction(refraction);

      return result;
    }

    // Export ////////////////////////////////////////////////////////////////

    MaterialPtr parseMaterial(const tinyxml2::XMLElement *node)
    {
      if( node == nullptr ) {
        return MaterialPtr();
      }

      if(        node->Attribute("type", "Mirror") != nullptr ) {
        return parseMirrorMaterial(node);
      } else if( node->Attribute("type", "Opaque") != nullptr ) {
        return parseOpaqueMaterial(node);
      } else if( node->Attribute("type", "Transparent") != nullptr ) {
        return parseTransparentMaterial(node);
      }

      return MaterialPtr();
    }

  } // namespace priv

} // namespace rt
