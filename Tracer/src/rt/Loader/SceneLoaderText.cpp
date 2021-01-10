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

#include <font8x8_basic.h>

#include "rt/Loader/SceneLoaderBase.h"
#include "rt/Object/Sphere.h"

#define FW  8
#define FH  8

namespace rt {

  namespace priv {

    // Import ////////////////////////////////////////////////////////////////

    MaterialPtr parseMaterial(const tinyxml2::XMLElement *node);

    // Implementation ////////////////////////////////////////////////////////

    Objects createSpheres(std::string text, const real_t radius,
                          const real_t dx, const real_t dz,
                          const MaterialPtr& material, const Transform& transform)
    {
      using size_type = std::string::size_type;

      rt::Objects objs;

      // Text ////////////////////////////////////////////////////////////////////

      if( text.empty() ) {
        return objs;
      }

      std::for_each(text.begin(), text.end(), [](char& ch) -> void {
        if( ch < 0x20  ||  ch > 0x7E ) {
          ch = 0x20;
        }
      });

      // Spheres /////////////////////////////////////////////////////////////////

      const rt::real_t  width = dx*static_cast<rt::real_t>(text.size()*FW - 1);
      const rt::real_t height = dz*static_cast<rt::real_t>(FH - 1);
      const rt::real_t     x0 = -width/rt::TWO;
      const rt::real_t     z0 = height/rt::TWO;

      for(size_type i = 0; i < text.size(); i++) {
        const size_type index = static_cast<size_type>(text[i]);

        for(size_type row = 0; row < FH; row++) {
          const uint8_t  bits = static_cast<uint8_t>(font8x8_basic[index][row]);
          const rt::real_t oz = z0 - dz*(row);

          rt::real_t ox = x0 + dx*(i*FW);
          for(size_type col = 0; col < FW; col++, ox += dx) {
            if( (bits & (1 << col)) == 0 ) {
              continue;
            }

            MaterialPtr myMaterial = material->copy();

            objs.push_back(rt::Sphere::create(transform*rt::Transform::translate(ox, 0, oz),
                                              myMaterial, radius));
          } // For Each Column
        } // For Each Row
      } // For Each Glyph

      return objs;
    }

    // Export ////////////////////////////////////////////////////////////////

    Objects parseText(const tinyxml2::XMLElement *node)
    {
      if( node == nullptr ) {
        return Objects();
      }

      bool myOk = false;

      const real_t dx = parseReal(node->FirstChildElement("dx"), &myOk);
      if( !myOk  ||  dx <= 0 ) {
        return Objects();
      }

      const real_t dz = parseReal(node->FirstChildElement("dz"), &myOk);
      if( !myOk  ||  dz <= 0 ) {
        return Objects();
      }

      const MaterialPtr material = parseMaterial(node->FirstChildElement("Material"));
      if( !material ) {
        return Objects();
      }

      const real_t radius = parseReal(node->FirstChildElement("Radius"), &myOk);
      if( !myOk  ||  radius <= 0 ) {
        return Objects();
      }

      const std::string text = parseString(node->FirstChildElement("String"), &myOk);
      if( !myOk  ||  text.size() < 1 ) {
        return Objects();
      }

      const Transform transform = parseTransform(node->FirstChildElement("Transform"), &myOk);
      if( !myOk ) {
        return Objects();
      }

      return createSpheres(text, radius, dx, dz, material, transform);
    }

  } // namespace priv

} // namespace rt
