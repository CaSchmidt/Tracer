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

#include "rt/Light/DiffuseAreaLight.h"
#include "rt/Light/DirectionalLight.h"
#include "rt/Light/PointLight.h"
#include "rt/Loader/SceneLoaderBase.h"
#include "rt/Material/OpaqueMaterial.h"
#if 0
#include "rt/Texture/FlatTexture.h"
#endif

namespace rt {

  namespace priv {

    // Implementation ////////////////////////////////////////////////////////

    LightPtr parseDiffuseAreaLight(const tinyxml2::XMLElement *node, const ObjectConsumer& add_object)
    {
      bool myOk = false;

      const Color Lemit = parseColor(node->FirstChildElement("Emittance"), &myOk, false);
      if( !myOk ) {
        return LightPtr();
      }

      ObjectPtr object = parseObject(node->FirstChildElement("Object"), true);
      if( !object ) {
        return LightPtr();
      }

      MaterialPtr material = OpaqueMaterial::create();
      if( !material ) {
        return LightPtr();
      }
#if 0
      OPAQUE(material)->setDiffuse(FlatTexture::create(Lemit));
      if( !material->haveTexture(0) ) {
        return LightPtr();
      }
#endif
      object->setMaterial(material);

      LightPtr light = DiffuseAreaLight::create(object.get(), Lemit);
      if( !light ) {
        return LightPtr();
      }
      object->setAreaLight(IAREALIGHT(light));
      add_object(object);

      const size_t numSamples = parseSize(node->FirstChildElement("NumSamples"), &myOk);
      if( myOk ) {
        light->setNumSamples(numSamples);
      }

      return light;
    }

    LightPtr parseDirectionalLight(const tinyxml2::XMLElement *node)
    {
      bool myOk = false;

      const Color L = parseColor(node->FirstChildElement("Radiance"), &myOk, false);
      if( !myOk ) {
        return LightPtr();
      }

      const Direction wi = parseDirection(node->FirstChildElement("Direction"), &myOk);
      if( !myOk ) {
        return LightPtr();
      }

      return DirectionalLight::create(n4::identity(), L, wi);
    }

    LightPtr parsePointLight(const tinyxml2::XMLElement *node)
    {
      bool myOk = false;

      const Color I = parseColor(node->FirstChildElement("Intensity"), &myOk, false);
      if( !myOk ) {
        return LightPtr();
      }

      const Vertex pos = parseVertex(node->FirstChildElement("Position"), &myOk);
      if( !myOk ) {
        return LightPtr();
      }

      const Matrix lightToWorld = n4::translate(pos.x, pos.y, pos.z);

      return PointLight::create(lightToWorld, I);
    }

    // Export ////////////////////////////////////////////////////////////////

    LightPtr parseLight(const tinyxml2::XMLElement *node, const ObjectConsumer& add_object)
    {
      if( node == nullptr ) {
        return LightPtr();
      }

      if(        node->Attribute("type", "DiffuseAreaLight") != nullptr ) {
        return parseDiffuseAreaLight(node, add_object);
      } else if( node->Attribute("type", "Directional") != nullptr ) {
        return parseDirectionalLight(node);
      } else if( node->Attribute("type", "Point") != nullptr ) {
        return parsePointLight(node);
      }

      return LightPtr();
    }

  } // namespace priv

} // namespace rt
