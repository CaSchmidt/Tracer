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

#include <cctype>
#include <cstdint>

#include <algorithm>
#include <charconv>
#include <limits>

#include <tinyxml2.h>

#include "rt/Loader/SceneLoader.h"

#include "rt/Light/ILight.h"
#include "rt/Loader/SceneLoaderBase.h"
#include "rt/Loader/SceneLoaderObject.h"
#include "rt/Loader/SceneLoaderStringUtil.h"
#include "rt/Renderer/IRenderer.h"
#include "rt/Scene/Scene.h"

namespace rt {

  namespace priv {

    // Imports ///////////////////////////////////////////////////////////////

    LightPtr parseLight(const tinyxml2::XMLElement *node, const ObjectConsumer& add_object);

    Objects parseText(const tinyxml2::XMLElement *node);

  } // namespace priv

  bool loadScene(Scene *scene, RenderOptions *options, const char *filename)
  {
    scene->clear();
    *options = RenderOptions();

    tinyxml2::XMLDocument doc;
    if( doc.LoadFile(filename) != tinyxml2::XML_SUCCESS ) {
      fprintf(stderr, "Unable to load XML scene \"%s\"!\n", filename);
      return false;
    }

    const tinyxml2::XMLElement *xml_Tracer = doc.FirstChildElement("Tracer");
    if( xml_Tracer == nullptr ) {
      fprintf(stderr, "Invalid XML scene!\n");
      return false;
    }

    bool ok = false;

    *options = RenderOptions::load(xml_Tracer, &ok);
    if( !ok ) {
      fprintf(stderr, "Unable to parse render options!\n");
      return false;
    }

    const tinyxml2::XMLElement *xml_Scene = xml_Tracer->FirstChildElement("Scene");
    if( xml_Scene == nullptr ) {
      fprintf(stderr, "Unable to initialize scene!\n");
      return false;
    }

    const priv::ObjectConsumer add_object = [&](ObjectPtr& o) -> void {
      scene->add(o);
    };

    const tinyxml2::XMLElement *node = xml_Scene->FirstChildElement();
    while( node != nullptr ) {
      if(        priv::compare(node->Name(), "BackgroundColor") ) {
        bool myOk{false};
        const Color bg = priv::parseColor(node, &myOk);
        if( !myOk ) {
          fprintf(stderr, "Unable to parse background color!");
          return false;
        }
        scene->setBackgroundColor(bg);
      } else if( priv::compare(node->Name(), "Light") ) {
        LightPtr light = priv::parseLight(node, add_object);
        if( !light ) {
          fprintf(stderr, "Unable to add light of type \"%s\"!\n", node->Attribute("type"));
          return false;
        }
        scene->add(light);
      } else if( priv::compare(node->Name(), "Object") ) {
        ObjectPtr object = priv::parseObject(node);
        if( !object ) {
          fprintf(stderr, "Unable to add object of type \"%s\"!\n", node->Attribute("type"));
          return false;
        }
        scene->add(object);
      } else if( priv::compare(node->Name(), "Text") ) {
        Objects objects = priv::parseText(node);
        if( objects.empty() ) {
          fprintf(stderr, "Unable to add text!\n");
          return false;
        }
        for(ObjectPtr& object : objects) {
          scene->add(object);
        }
      }

      node = node->NextSiblingElement();
    }

    return true;
  }

} // namespace rt
