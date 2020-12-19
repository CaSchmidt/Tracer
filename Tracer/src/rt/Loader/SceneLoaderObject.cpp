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

      const real_T height = parseReal(node->FirstChildElement("Height"), &myOk);
      if( !myOk  ||  height <= 0 ) {
        return ObjectPtr();
      }

      MaterialPtr material = parseMaterial(node->FirstChildElement("Material"));
      if( !material ) {
        return ObjectPtr();
      }

      const real_T radius = parseReal(node->FirstChildElement("Radius"), &myOk);
      if( !myOk  ||  radius <= 0 ) {
        return ObjectPtr();
      }

      Transformf transform = parseTransform(node->FirstChildElement("Transform"), &myOk);
      if( !myOk ) {
        return ObjectPtr();
      }

      return Cylinder::create(transform, material, height, radius);
    }

    ObjectPtr parseDisk(const tinyxml2::XMLElement *node)
    {
      bool myOk = false;

      MaterialPtr material = parseMaterial(node->FirstChildElement("Material"));
      if( !material ) {
        return ObjectPtr();
      }

      const real_T radius = parseReal(node->FirstChildElement("Radius"), &myOk);
      if( !myOk  ||  radius <= 0 ) {
        return ObjectPtr();
      }

      Transformf transform = parseTransform(node->FirstChildElement("Transform"), &myOk);
      if( !myOk ) {
        return ObjectPtr();
      }

      return Disk::create(transform, material, radius);
    }

    ObjectPtr parsePillar(const tinyxml2::XMLElement *node)
    {
      bool myOk = false;

      const real_T height = parseReal(node->FirstChildElement("Height"), &myOk);
      if( !myOk  ||  height <= 0 ) {
        return ObjectPtr();
      }

      MaterialPtr material = parseMaterial(node->FirstChildElement("Material"));
      if( !material ) {
        return ObjectPtr();
      }
      MaterialPtr materialBottom = material->copy();
      MaterialPtr materialTop    = material->copy();

      const real_T radius = parseReal(node->FirstChildElement("Radius"), &myOk);
      if( !myOk  ||  radius <= 0 ) {
        return ObjectPtr();
      }

      Transformf transform = parseTransform(node->FirstChildElement("Transform"), &myOk);
      if( !myOk ) {
        return ObjectPtr();
      }

      ObjectPtr groupPtr = Group::create(transform);
      Group       *group = dynamic_cast<Group*>(groupPtr.get());

      // (1) Body ////////////////////////////////////////////////////////////

      ObjectPtr body = Cylinder::create(Transformf(), material, height, radius);
      group->add(body);

      // (2) Top /////////////////////////////////////////////////////////////

      ObjectPtr top = Disk::create(Transformf::translate({0, 0, height/2}), materialTop, radius);
      group->add(top);

      // (3) Bottom //////////////////////////////////////////////////////////

      const Transformf xfrmBottom =
          Transformf::translate({0, 0, -height/2})*
          Transformf::rotateZYXbyPI2(0, 0, 2);
      ObjectPtr bottom = Disk::create(xfrmBottom, materialBottom, radius);
      group->add(bottom);

      return groupPtr;
    }

    ObjectPtr parsePlane(const tinyxml2::XMLElement *node)
    {
      bool myOk = false;

      const real_T height = parseReal(node->FirstChildElement("Height"), &myOk);
      if( !myOk  ||  height <= 0 ) {
        return ObjectPtr();
      }

      MaterialPtr material = parseMaterial(node->FirstChildElement("Material"));
      if( !material ) {
        return ObjectPtr();
      }

      Transformf transform = parseTransform(node->FirstChildElement("Transform"), &myOk);
      if( !myOk ) {
        return ObjectPtr();
      }

      const real_T width = parseReal(node->FirstChildElement("Width"), &myOk);
      if( !myOk  ||  width <= 0 ) {
        return ObjectPtr();
      }

      return Plane::create(transform, material, width, height);
    }

    ObjectPtr parseSphere(const tinyxml2::XMLElement *node)
    {
      bool myOk = false;

      MaterialPtr material = parseMaterial(node->FirstChildElement("Material"));
      if( !material ) {
        return ObjectPtr();
      }

      const real_T radius = parseReal(node->FirstChildElement("Radius"), &myOk);
      if( !myOk  ||  radius <= 0 ) {
        return ObjectPtr();
      }

      Transformf transform = parseTransform(node->FirstChildElement("Transform"), &myOk);
      if( !myOk ) {
        return ObjectPtr();
      }

      return Sphere::create(transform, material, radius);
    }

    // Export ////////////////////////////////////////////////////////////////

    ObjectPtr parseObject(const tinyxml2::XMLElement *node)
    {
      if( node == nullptr ) {
        return ObjectPtr();
      }

      if(        node->Attribute("type", "Cylinder") != nullptr ) {
        return parseCylinder(node);
      } else if( node->Attribute("type", "Disk") != nullptr ) {
        return parseDisk(node);
      } else if( node->Attribute("type", "Pillar") != nullptr ) {
        return parsePillar(node);
      } else if( node->Attribute("type", "Plane") != nullptr ) {
        return parsePlane(node);
      } else if( node->Attribute("type", "Sphere") != nullptr ) {
        return parseSphere(node);
      }

      return ObjectPtr();
    }

  } // namespace priv

} // namespace rt
