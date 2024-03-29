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

#include <tinyxml2.h>

#include "rt/Loader/SceneLoaderBase.h"
#include "rt/Object/Cylinder.h"
#include "rt/Object/Disk.h"
#include "rt/Object/Group.h"
#include "rt/Object/Plane.h"
#include "rt/Object/Sphere.h"

namespace rt {

  namespace priv {

    // Import ////////////////////////////////////////////////////////////////

    MaterialPtr parseMaterial(const tinyxml2::XMLElement *node);

    // Implementation ////////////////////////////////////////////////////////

    ObjectPtr parseCylinder(const tinyxml2::XMLElement *node)
    {
      bool myOk = false;

      const real_t height = parseReal(node->FirstChildElement("Height"), &myOk);
      if( !myOk ) {
        return ObjectPtr();
      }

      MaterialPtr material = parseMaterial(node->FirstChildElement("Material"));
      if( !material ) {
        return ObjectPtr();
      }

      const real_t radius = parseReal(node->FirstChildElement("Radius"), &myOk);
      if( !myOk ) {
        return ObjectPtr();
      }

      Transform transform = parseTransform(node->FirstChildElement("Transform"), &myOk);
      if( !myOk ) {
        return ObjectPtr();
      }

      ObjectPtr object = Cylinder::create(transform, height, radius);
      object->setMaterial(material);

      return object;
    }

    ObjectPtr parseDisk(const tinyxml2::XMLElement *node, const bool mat_is_opt)
    {
      bool myOk = false;

      MaterialPtr material = parseMaterial(node->FirstChildElement("Material"));
      if( !mat_is_opt  &&  !material ) {
        return ObjectPtr();
      }

      const real_t radius = parseReal(node->FirstChildElement("Radius"), &myOk);
      if( !myOk ) {
        return ObjectPtr();
      }

      Transform transform = parseTransform(node->FirstChildElement("Transform"), &myOk);
      if( !myOk ) {
        return ObjectPtr();
      }

      ObjectPtr object = Disk::create(transform, radius);
      object->setMaterial(material);

      return object;
    }

    ObjectPtr parsePillar(const tinyxml2::XMLElement *node)
    {
      bool myOk = false;

      const real_t height = parseReal(node->FirstChildElement("Height"), &myOk);
      if( !myOk  ||  height <= 0 ) {
        return ObjectPtr();
      }

      MaterialPtr material = parseMaterial(node->FirstChildElement("Material"));
      if( !material ) {
        return ObjectPtr();
      }
      MaterialPtr materialBottom = material->copy();
      MaterialPtr materialTop    = material->copy();

      const real_t radius = parseReal(node->FirstChildElement("Radius"), &myOk);
      if( !myOk  ||  radius <= 0 ) {
        return ObjectPtr();
      }

      Transform transform = parseTransform(node->FirstChildElement("Transform"), &myOk);
      if( !myOk ) {
        return ObjectPtr();
      }

      ObjectPtr groupPtr = Group::create(transform);
      Group       *group = GROUP(groupPtr);

      // (1) Body ////////////////////////////////////////////////////////////

      ObjectPtr body = Cylinder::create(Transform(), height, radius);
      body->setMaterial(material);
      group->add(body);

      // (2) Top /////////////////////////////////////////////////////////////

      ObjectPtr top = Disk::create(Transform::translate(0, 0, height/2), radius);
      top->setMaterial(materialTop);
      group->add(top);

      // (3) Bottom //////////////////////////////////////////////////////////

      const Transform xfrmBottom =
          Transform::translate(0, 0, -height/2)*
          Transform::rotateZYXbyPI2(0, 0, 2);
      ObjectPtr bottom = Disk::create(xfrmBottom, radius);
      bottom->setMaterial(materialBottom);
      group->add(bottom);

      return groupPtr;
    }

    ObjectPtr parsePlane(const tinyxml2::XMLElement *node, const bool mat_is_opt)
    {
      bool myOk = false;

      const real_t height = parseReal(node->FirstChildElement("Height"), &myOk);
      if( !myOk ) {
        return ObjectPtr();
      }

      MaterialPtr material = parseMaterial(node->FirstChildElement("Material"));
      if( !mat_is_opt  &&  !material ) {
        return ObjectPtr();
      }

      Transform transform = parseTransform(node->FirstChildElement("Transform"), &myOk);
      if( !myOk ) {
        return ObjectPtr();
      }

      const real_t width = parseReal(node->FirstChildElement("Width"), &myOk);
      if( !myOk ) {
        return ObjectPtr();
      }

      ObjectPtr object = Plane::create(transform, width, height);
      object->setMaterial(material);

      return object;
    }

    ObjectPtr parseSphere(const tinyxml2::XMLElement *node, const bool mat_is_opt)
    {
      bool myOk = false;

      MaterialPtr material = parseMaterial(node->FirstChildElement("Material"));
      if( !mat_is_opt  &&  !material ) {
        return ObjectPtr();
      }

      const real_t radius = parseReal(node->FirstChildElement("Radius"), &myOk);
      if( !myOk ) {
        return ObjectPtr();
      }

      Transform transform = parseTransform(node->FirstChildElement("Transform"), &myOk);
      if( !myOk ) {
        return ObjectPtr();
      }

      ObjectPtr object = Sphere::create(transform, radius);
      object->setMaterial(material);

      return object;
    }

    // Export ////////////////////////////////////////////////////////////////

    ObjectPtr parseObject(const tinyxml2::XMLElement *node, const bool mat_is_opt)
    {
      if( node == nullptr ) {
        return ObjectPtr();
      }

      if(        node->Attribute("type", "Cylinder") != nullptr ) {
        return parseCylinder(node);
      } else if( node->Attribute("type", "Disk") != nullptr ) {
        return parseDisk(node, mat_is_opt);
      } else if( node->Attribute("type", "Pillar") != nullptr ) {
        return parsePillar(node);
      } else if( node->Attribute("type", "Plane") != nullptr ) {
        return parsePlane(node, mat_is_opt);
      } else if( node->Attribute("type", "Sphere") != nullptr ) {
        return parseSphere(node, mat_is_opt);
      }

      return ObjectPtr();
    }

  } // namespace priv

} // namespace rt
