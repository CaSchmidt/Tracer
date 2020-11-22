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
#include "rt/RenderOptions.h"

namespace rt {

  namespace priv {

    RenderOptions parseOptions(const tinyxml2::XMLElement *node, bool *ok)
    {
      if( ok != nullptr ) {
        *ok = false;
      }

      if( node == nullptr ) {
        return RenderOptions();
      }

      bool myOk = false;
      RenderOptions opts;

      opts.width = parseSize(node->FirstChildElement("Width"), &myOk);
      if( !myOk ) {
        return RenderOptions();
      }

      opts.height = parseSize(node->FirstChildElement("Height"), &myOk);
      if( !myOk ) {
        return RenderOptions();
      }

      opts.fov_rad = parseAngle(node->FirstChildElement("FoV"), &myOk);
      if( !myOk ) {
        return RenderOptions();
      }

      opts.worldToScreen = parseReal(node->FirstChildElement("WorldToScreen"), &myOk);
      if( !myOk ) {
        return RenderOptions();
      }

      opts.aperture = parseReal(node->FirstChildElement("Aperture"), &myOk);
      if( !myOk ) {
        opts.aperture = 0;
      }

      opts.focus = parseReal(node->FirstChildElement("Focus"), &myOk);
      if( !myOk ) {
        opts.focus = 0;
      }

      opts.backgroundColor = parseColor(node->FirstChildElement("BackgroundColor"), &myOk);
      if( !myOk ) {
        return RenderOptions();
      }

      opts.eye = parseVertex(node->FirstChildElement("Eye"), &myOk);
      if( !myOk ) {
        return RenderOptions();
      }

      opts.lookAt = parseVertex(node->FirstChildElement("LookAt"), &myOk);
      if( !myOk ) {
        return RenderOptions();
      }

      opts.cameraUp = parseNormal(node->FirstChildElement("CameraUp"), &myOk);
      if( !myOk ) {
        return RenderOptions();
      }

      if( ok != nullptr ) {
        *ok = true;
      }

      return opts;
    }

  } // namespace priv

} // namespace rt
