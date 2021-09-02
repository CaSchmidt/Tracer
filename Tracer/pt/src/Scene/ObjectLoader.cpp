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

#include "pt/Scene/Object.h"

#include "rt/Loader/SceneLoaderBase.h"
#include "rt/Loader/SceneLoaderStringUtil.h"

namespace pt {

  ////// public //////////////////////////////////////////////////////////////

  bool Object::isObject(const tinyxml2::XMLElement *elem)
  {
    return elem != nullptr  &&  rt::priv::compare(elem->Value(), "Object");
  }

  ObjectPtr Object::load(const tinyxml2::XMLElement *elem)
  {
    if( !isObject(elem) ) {
      return ObjectPtr();
    }

    bool ok = false;

    // (1) Load Transform ////////////////////////////////////////////////////

    const rt::Transform objectToWorld = rt::priv::parseTransform(elem->FirstChildElement("Transform"), &ok);
    if( !ok ) {
      return ObjectPtr();
    }

    // (2) Load Shapes ///////////////////////////////////////////////////////

    ObjectPtr object;
    if(        isBox(elem) ) {
      object = loadBox(elem, objectToWorld);
    } else if( isInvertedBox(elem) ) {
      object = loadInvertedBox(elem, objectToWorld);
    } else if( isPillar(elem) ) {
      object = loadPillar(elem, objectToWorld);
    } else {
      object = loadShapes(elem, objectToWorld);
    }

    if( !object ) {
      return ObjectPtr();
    }

    // (3) Load BSDF /////////////////////////////////////////////////////////

    BSDFPtr bsdf = IBSDF::load(elem->FirstChildElement("BSDF"));
    if( !bsdf ) {
      return ObjectPtr();
    }

    // (4) Load Texture(s) ///////////////////////////////////////////////////

    for(const tinyxml2::XMLElement *xml_Texture = elem->FirstChildElement("Texture");
        xml_Texture != nullptr; xml_Texture = xml_Texture->NextSiblingElement("Texture")) {
      if( !rt::ITexture::isTexture(xml_Texture) ) { // Superfluous!
        continue;
      }
      rt::TexturePtr texture = rt::ITexture::load(xml_Texture);
      if( !texture ) {
        return ObjectPtr();
      }
      const rt::size_t id = rt::ITexture::readId(xml_Texture);
      object->setTexture(id, texture);
    }

    // (5) Load Emittance ////////////////////////////////////////////////////

    const rt::Color emitColor = rt::priv::parseColor(elem->FirstChildElement("EmitColor"), &ok);
    if( !ok ) {
      return ObjectPtr();
    }
    object->setEmissiveColor(emitColor);

    const rt::real_t emitScale = rt::priv::parseReal(elem->FirstChildElement("EmitScale"), &ok);
    if( !ok ) {
      return ObjectPtr();
    }
    object->setEmissiveScale(emitScale);

    // Done! /////////////////////////////////////////////////////////////////

    return object;
  }

  ////// private /////////////////////////////////////////////////////////////

  bool Object::isBox(const tinyxml2::XMLElement *elem)
  {
    return isObject(elem)  &&  elem->Attribute("type", "Box") != nullptr;
  }

  bool Object::isInvertedBox(const tinyxml2::XMLElement *elem)
  {
    return isObject(elem)  &&  elem->Attribute("type", "InvertedBox") != nullptr;
  }

  bool Object::isPillar(const tinyxml2::XMLElement *elem)
  {
    return isObject(elem)  &&  elem->Attribute("type", "Pillar") != nullptr;
  }

  ObjectPtr Object::loadBox(const tinyxml2::XMLElement *elem,
                            const rt::Transform& objectToWorld)
  {
    if( !isBox(elem) ) {
      return ObjectPtr();
    }

    bool ok = false;

    const rt::real_t dimx = rt::priv::parseReal(elem->FirstChildElement("DimX"), &ok);
    if( !ok ) {
      return ObjectPtr();
    }

    const rt::real_t dimy = rt::priv::parseReal(elem->FirstChildElement("DimY"), &ok);
    if( !ok ) {
      return ObjectPtr();
    }

    const rt::real_t dimz = rt::priv::parseReal(elem->FirstChildElement("DimZ"), &ok);
    if( !ok ) {
      return ObjectPtr();
    }

    return createBox(objectToWorld, dimx, dimy, dimz);
  }

  ObjectPtr Object::loadInvertedBox(const tinyxml2::XMLElement *elem,
                                    const rt::Transform& objectToWorld)
  {
    if( !isInvertedBox(elem) ) {
      return ObjectPtr();
    }

    bool ok = false;

    const rt::real_t dimx = rt::priv::parseReal(elem->FirstChildElement("DimX"), &ok);
    if( !ok ) {
      return ObjectPtr();
    }

    const rt::real_t dimy = rt::priv::parseReal(elem->FirstChildElement("DimY"), &ok);
    if( !ok ) {
      return ObjectPtr();
    }

    const rt::real_t dimz = rt::priv::parseReal(elem->FirstChildElement("DimZ"), &ok);
    if( !ok ) {
      return ObjectPtr();
    }

    return createInvertedBox(objectToWorld, dimx, dimy, dimz);
  }

  ObjectPtr Object::loadPillar(const tinyxml2::XMLElement *elem,
                               const rt::Transform& objectToWorld)
  {
    if( !isPillar(elem) ) {
      return ObjectPtr();
    }

    bool ok = false;

    const rt::real_t height = rt::priv::parseReal(elem->FirstChildElement("Height"), &ok);
    if( !ok ) {
      return ObjectPtr();
    }

    const rt::real_t radius = rt::priv::parseReal(elem->FirstChildElement("Radius"), &ok);
    if( !ok ) {
      return ObjectPtr();
    }

    return createPillar(objectToWorld, height, radius);
  }

  ObjectPtr Object::loadShapes(const tinyxml2::XMLElement *elem,
                               const rt::Transform& objectToWorld)
  {
    if( !isObject(elem) ) {
      return ObjectPtr();
    }

    ObjectPtr object = create(objectToWorld);

    for(const tinyxml2::XMLElement *child = elem->FirstChildElement("Shape");
        child != nullptr; child = child->NextSiblingElement("Shape")) {
      if( !IShape::isShape(child) ) {
        continue;
      }
      ShapePtr shape = IShape::load(child);
      if( !shape ) {
        return ObjectPtr();
      }
      object->add(shape);
    }

    return object;
  }

} // namespace pt
