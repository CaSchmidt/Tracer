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

#include "rt/Renderer/RenderOptions.h"

#include "rt/Loader/SceneLoaderBase.h"

namespace rt {

  RenderOptions RenderOptions::load(const tinyxml2::XMLElement *parent, bool *ok)
  {
    if( ok != nullptr ) {
      *ok = false;
    }

    const tinyxml2::XMLElement *xml_Options = parent->FirstChildElement("Options");
    if( xml_Options == nullptr ) {
      return RenderOptions();
    }

    bool myOk = false;
    RenderOptions result;

    result.fov_rad = priv::parseAngle(xml_Options->FirstChildElement("FoV"), &myOk);
    if( !myOk ) {
      return RenderOptions();
    }

    result.worldToScreen = priv::parseReal(xml_Options->FirstChildElement("WorldToScreen"), &myOk);
    if( !myOk ) {
      return RenderOptions();
    }

    result.aperture = priv::parseReal(xml_Options->FirstChildElement("Aperture"), &myOk);
    if( !myOk ) {
      result.aperture = 0;
    }

    result.focus = priv::parseReal(xml_Options->FirstChildElement("Focus"), &myOk);
    if( !myOk ) {
      result.focus = 0;
    }

    result.eye = priv::parseVertex(xml_Options->FirstChildElement("Eye"), &myOk);
    if( !myOk ) {
      return RenderOptions();
    }

    result.lookAt = priv::parseVertex(xml_Options->FirstChildElement("LookAt"), &myOk);
    if( !myOk ) {
      return RenderOptions();
    }

    result.cameraUp = priv::parseDirection(xml_Options->FirstChildElement("CameraUp"), &myOk);
    if( !myOk ) {
      return RenderOptions();
    }

    if( ok != nullptr ) {
      *ok = true;
    }

    return result;
  }

} // namespace rt
