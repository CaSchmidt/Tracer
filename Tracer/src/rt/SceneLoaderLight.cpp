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
#include "rt/Light/DirectionalLight.h"
#include "rt/Light/PointLight.h"

namespace rt {

  namespace priv {

    // Implementation ////////////////////////////////////////////////////////

    LightSourcePtr parseDirectionalLight(const tinyxml2::XMLElement *node)
    {
      bool myOk = false;

      const Color3f EL = parseColor(node->FirstChildElement("Irradiance"), &myOk, false);
      if( !myOk ) {
        return LightSourcePtr();
      }

      const Normal3f dir = parseNormal(node->FirstChildElement("Direction"), &myOk);
      if( !myOk ) {
        return LightSourcePtr();
      }

      return DirectionalLight::create(EL, dir);
    }

    LightSourcePtr parsePointLight(const tinyxml2::XMLElement *node)
    {
      bool myOk = false;

      const Color3f IL = parseColor(node->FirstChildElement("Intensity"), &myOk, false);
      if( !myOk ) {
        return LightSourcePtr();
      }

      const Vertex3f pos = parseVertex(node->FirstChildElement("Position"), &myOk);
      if( !myOk ) {
        return LightSourcePtr();
      }

      return PointLight::create(IL, pos);
    }

    // Export ////////////////////////////////////////////////////////////////

    LightSourcePtr parseLight(const tinyxml2::XMLElement *node)
    {
      if( node == nullptr ) {
        return LightSourcePtr();
      }

      if(        node->Attribute("type", "Directional") != nullptr ) {
        return parseDirectionalLight(node);
      } else if( node->Attribute("type", "Point") != nullptr ) {
        return parsePointLight(node);
      }

      return LightSourcePtr();
    }

  } // namespace priv

} // namespace rt