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

#ifndef SCENELOADERBASE_H
#define SCENELOADERBASE_H

#include <charconv>
#include <string>
#include <type_traits>

#include <tinyxml2.h>

#include "priv/rt/SceneLoaderStringUtil.h"
#include "rt/Types.h"

namespace rt {

  namespace priv {

    template<typename T>
    inline T parseNodeAsInt(const tinyxml2::XMLElement *node, bool *ok = nullptr)
    {
      if( ok != nullptr ) {
        *ok = false;
      }

      if( node == nullptr  ||  length(node->GetText()) < 1 ) {
        return T();
      }

      int base = 10;
      const char *s = skipSpace(node->GetText());
      if( length(s) > 0  &&  s[0] == '+' ) {
        s += 1;
      }
      if( isHexPrefix(s) ) {
        base = 16;
        s += 2;
      }

      std::enable_if_t<std::is_integral_v<T>,T> value;
      const std::from_chars_result result = std::from_chars(s, s + length(s), value, base);
      if( result.ec != std::errc() ) {
        return T();
      }

      if( ok != nullptr ) {
        *ok = true;
      }

      return value;
    }

    template<typename T>
    inline T parseNodeAsFloat(const tinyxml2::XMLElement *node, bool *ok = nullptr)
    {
      if( ok != nullptr ) {
        *ok = false;
      }

      if( node == nullptr  ||  length(node->GetText()) < 1 ) {
        return T();
      }

      std::chars_format fmt = std::chars_format::general;
      const char *s = skipSpace(node->GetText());
      if( length(s) > 0  &&  s[0] == '+' ) {
        s += 1;
      }
      if( isHexPrefix(s) ) {
        fmt = std::chars_format::hex;
        s += 2;
      }

      std::enable_if_t<std::is_floating_point_v<T>,T> value;
      const std::from_chars_result result = std::from_chars(s, s + length(s), value, fmt);
      if( result.ec != std::errc() ) {
        return T();
      }

      if( ok != nullptr ) {
        *ok = true;
      }

      return value;
    }

    template<typename T>
    T parseVector3D(const tinyxml2::XMLElement *node,
                    const char *id0, const char *id1, const char *id2,
                    bool *ok = nullptr)
    {
      using value_type = typename T::value_type;

      if( ok != nullptr ) {
        *ok = false;
      }

      if( node == nullptr ) {
        return T();
      }

      bool myOk;
      T result;

      myOk = false;
      result[0] = parseNodeAsFloat<value_type>(node->FirstChildElement(id0), &myOk);
      if( !myOk ) {
        return T();
      }

      myOk = false;
      result[1] = parseNodeAsFloat<value_type>(node->FirstChildElement(id1), &myOk);
      if( !myOk ) {
        return T();
      }

      myOk = false;
      result[2] = parseNodeAsFloat<value_type>(node->FirstChildElement(id2), &myOk);
      if( !myOk ) {
        return T();
      }

      if( ok != nullptr ) {
        *ok = true;
      }

      return result;
    }

    real_T parseAngle(const tinyxml2::XMLElement *node, bool *ok);

    Color3f parseColor(const tinyxml2::XMLElement *node, bool *ok, const bool clamp = true);

    Normal3f parseNormal(const tinyxml2::XMLElement *node, bool *ok);

    Matrix3f parseRotation(const tinyxml2::XMLElement *node, bool *ok);

    std::size_t parseSize(const tinyxml2::XMLElement *node, bool *ok);

    std::string parseString(const tinyxml2::XMLElement *node, bool *ok);

    Transformf parseTransform(const tinyxml2::XMLElement *node, bool *ok);

    Vertex3f parseVertex(const tinyxml2::XMLElement *node, bool *ok);

  } // namespace priv

} // namespace rt

#endif // SCENELOADERBASE_H
