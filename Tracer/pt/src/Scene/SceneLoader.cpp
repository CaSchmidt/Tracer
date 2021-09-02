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

#include "pt/Scene/Scene.h"

#include "rt/Loader/SceneLoaderBase.h"
#include "rt/Loader/SceneLoaderStringUtil.h"
#include "rt/Renderer/RenderOptions.h"

namespace pt {

  bool Scene::isScene(const tinyxml2::XMLElement *elem)
  {
    return elem != nullptr  &&  rt::priv::compare(elem->Value(), "PathTracerScene");
  }

  bool Scene::isScene(const char *filename)
  {
    tinyxml2::XMLDocument doc;
    if( doc.LoadFile(filename) != tinyxml2::XML_SUCCESS ) {
      return false;
    }
    return isScene(doc.FirstChildElement("PathTracerScene"));
  }

  bool Scene::load(Scene *scene, rt::RenderOptions *options, const char *filename)
  {
    using MyElem = tinyxml2::XMLElement;

    scene->clear();
    *options = rt::RenderOptions();

    tinyxml2::XMLDocument doc;
    if( doc.LoadFile(filename) != tinyxml2::XML_SUCCESS ) {
      fprintf(stderr, "Unable to load XML scene \"%s\"!\n", filename);
      return false;
    }

    const MyElem *xml_PathTracerScene = doc.FirstChildElement("PathTracerScene");
    if( !isScene(xml_PathTracerScene) ) {
      fprintf(stderr, "Invalid XML scene \"%s\"!\n", filename);
      return false;
    }

    bool ok = false;

    *options = rt::RenderOptions::load(xml_PathTracerScene, &ok);
    if( !ok ) {
      fprintf(stderr, "Unable to load <RenderOptions>!\n");
      return false;
    }

    const rt::Color backgroundColor = rt::priv::parseColor(xml_PathTracerScene->FirstChildElement("BackgroundColor"), &ok);
    if( !ok ) {
      fprintf(stderr, "Unable to load <BackgroundColor>!\n");
      return false;
    }
    scene->setBackgroundColor(backgroundColor);

    for(const MyElem *child = xml_PathTracerScene->FirstChildElement("Object");
        child != nullptr; child = child->NextSiblingElement("Object")) {
      if( !Object::isObject(child) ) {
        continue;
      }
      ObjectPtr object = Object::load(child);
      if( !object ) {
        fprintf(stderr, "Unable to load <Object> at line \"%d\"!\n", child->GetLineNum());
        return false;
      }
      scene->add(object);
    }

    return true;
  }

} // namespace pt
