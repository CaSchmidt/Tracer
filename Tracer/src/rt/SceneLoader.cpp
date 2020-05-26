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

#include "rt/SceneLoader.h"

#include "math/Solver.h"
#include "rt/Renderer.h"

namespace rt {

  namespace priv {

    template<typename CharT>
    inline bool isSpace(const CharT& ch)
    {
      return std::isspace(static_cast<int>(ch)) != 0;
    }

    template<typename CharT>
    constexpr std::size_t length(const CharT *s)
    {
      return s != nullptr
          ? std::char_traits<CharT>::length(s)
          : 0;
    }

    template<typename CharT>
    constexpr bool compare(const CharT *s1, const CharT *s2)
    {
      constexpr std::size_t l1 = length(s1);
      constexpr std::size_t l2 = length(s2);
      return l1 > 0  &&  l1 == l2
          ? std::char_traits<CharT>::compare(s1, s2, l1)
          : false;
    }

    template<typename CharT>
    constexpr bool isHexPrefix(const CharT *s)
    {
      constexpr CharT digit_0 = static_cast<CharT>('0');
      constexpr CharT  char_x = static_cast<CharT>('x');
      constexpr CharT  char_X = static_cast<CharT>('X');
      return length(s) > 2  &&  s[0] == digit_0  &&  (s[1] == char_x  ||  s[1] == char_X);
    }

    template<typename CharT>
    inline const CharT *skipSpace(const CharT *s)
    {
      return std::find_if_not(s, s + length(s), [](const CharT c) -> bool {
        return isSpace(c);
      });
    }

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

    real_T parseAngle(const tinyxml2::XMLElement *node, bool *ok = nullptr)
    {
      return math::radian(parseNodeAsFloat<real_T>(node, ok));
    }

    Color3f parseColor(const tinyxml2::XMLElement *node, bool *ok = nullptr, const bool clamp = true)
    {
      const Color3f result = parseVector3D<Color3f>(node, "r", "g", "b", ok);
      return clamp
          ? Color3f(cs::clamp(result, 0, 1))
          : Color3f(cs::max(result, 0));
    }

    Normal3f parseNormal(const tinyxml2::XMLElement *node, bool *ok = nullptr)
    {
      return cs::normalize(parseVector3D<Normal3f>(node, "x", "y", "z", ok));
    }

    dim_T parseSize(const tinyxml2::XMLElement *node, bool *ok = nullptr)
    {
      return parseNodeAsInt<dim_T>(node, ok);
    }

    Vertex3f parseVertex(const tinyxml2::XMLElement *node, bool *ok = nullptr)
    {
      return parseVector3D<Vertex3f>(node, "x", "y", "z", ok);
    }

    RenderOptions parseOptions(const tinyxml2::XMLElement *xml_Options, bool *ok = nullptr)
    {
      if( ok != nullptr ) {
        *ok = false;
      }

      if( xml_Options == nullptr ) {
        return RenderOptions();
      }

      bool myOk;
      RenderOptions opts;

      myOk = false;
      opts.width = parseSize(xml_Options->FirstChildElement("width"), &myOk);
      if( !myOk ) {
        return RenderOptions();
      }

      myOk = false;
      opts.height = parseSize(xml_Options->FirstChildElement("height"), &myOk);
      if( !myOk ) {
        return RenderOptions();
      }

      myOk = false;
      opts.fov_rad = parseAngle(xml_Options->FirstChildElement("fov"), &myOk);
      if( !myOk ) {
        return RenderOptions();
      }

      myOk = false;
      opts.backgroundColor = parseColor(xml_Options->FirstChildElement("backgroundColor"), &myOk);
      if( !myOk ) {
        return RenderOptions();
      }

      myOk = false;
      opts.eye = parseVertex(xml_Options->FirstChildElement("eye"), &myOk);
      if( !myOk ) {
        return RenderOptions();
      }

      myOk = false;
      opts.lookAt = parseVertex(xml_Options->FirstChildElement("lookAt"), &myOk);
      if( !myOk ) {
        return RenderOptions();
      }

      myOk = false;
      opts.cameraUp = parseNormal(xml_Options->FirstChildElement("cameraUp"), &myOk);
      if( !myOk ) {
        return RenderOptions();
      }

      if( ok != nullptr ) {
        *ok = true;
      }

      return opts;
    }

  } // namespace priv

  bool loadScene(Renderer& renderer, const char *filename)
  {
    renderer.clear();

    tinyxml2::XMLDocument doc;
    if( doc.LoadFile(filename) != tinyxml2::XML_SUCCESS ) {
      return false;
    }

    const tinyxml2::XMLElement *xml_Tracer = doc.FirstChildElement("Tracer");
    if( xml_Tracer == nullptr ) {
      return false;
    }

    return true;
  }

} // namespace rt
