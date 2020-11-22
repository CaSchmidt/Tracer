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

#include "math/Solver.h"
#include "rt/Loader/SceneLoaderStringUtil.h"

namespace rt {

  namespace priv {

    ////// Implementation ////////////////////////////////////////////////////

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

    ////// Interface /////////////////////////////////////////////////////////

    real_T parseAngle(const tinyxml2::XMLElement *node, bool *ok)
    {
      return math::radian(parseNodeAsFloat<real_T>(node, ok));
    }

    Color3f parseColor(const tinyxml2::XMLElement *node, bool *ok, const bool clamp)
    {
      const Color3f result = parseVector3D<Color3f>(node, "r", "g", "b", ok);
      return clamp
          ? Color3f(cs::clamp(result, 0, 1))
          : Color3f(cs::max(result, 0));
    }

    Normal3f parseNormal(const tinyxml2::XMLElement *node, bool *ok)
    {
      return cs::normalize(parseVector3D<Normal3f>(node, "x", "y", "z", ok));
    }

    real_T parseReal(const tinyxml2::XMLElement *node, bool *ok)
    {
      return parseNodeAsFloat<real_T>(node, ok);
    }

    Matrix3f parseRotation(const tinyxml2::XMLElement *node, bool *ok)
    {
      if( ok != nullptr ) {
        *ok = false;
      }

      if( node == nullptr ) {
        return Matrix3f();
      }

      bool myOk = false;

      real_T rx = 0;
      if( node->FirstChildElement("rx") != nullptr ) {
        rx = parseAngle(node->FirstChildElement("rx"), &myOk);
        if( !myOk ) {
          return Matrix3f();
        }
      }

      real_T ry = 0;
      if( node->FirstChildElement("ry") != nullptr ) {
        ry = parseAngle(node->FirstChildElement("ry"), &myOk);
        if( !myOk ) {
          return Matrix3f();
        }
      }

      real_T rz = 0;
      if( node->FirstChildElement("rz") != nullptr ) {
        rz = parseAngle(node->FirstChildElement("rz"), &myOk);
        if( !myOk ) {
          return Matrix3f();
        }
      }

      if( ok != nullptr ) {
        *ok = true;
      }

      return geom::rotateZ(rz)*geom::rotateY(ry)*geom::rotateX(rx);
    }

    std::size_t parseSize(const tinyxml2::XMLElement *node, bool *ok)
    {
      return parseNodeAsInt<std::size_t>(node, ok);
    }

    std::string parseString(const tinyxml2::XMLElement *node, bool *ok)
    {
      if( ok != nullptr ) {
        *ok = false;
      }

      if( node == nullptr ) {
        return std::string();
      }

      if( node->GetText() == nullptr ) {
        return std::string();
      }

      if( ok != nullptr ) {
        *ok = true;
      }

      return std::string(node->GetText());
    }

    Transformf parseTransform(const tinyxml2::XMLElement *node, bool *ok)
    {
      if( ok != nullptr ) {
        *ok = false;
      }

      if( node == nullptr ) {
        return Transformf();
      }

      bool myOk = false;

      Vertex3f t = 0;
      if( node->FirstChildElement("Translate") != nullptr ) {
        t = parseVertex(node->FirstChildElement("Translate"), &myOk);
        if( !myOk ) {
          return Transformf();
        }
      }

      Matrix3f R = geom::identity<real_T>();
      if( node->FirstChildElement("Rotate") != nullptr ) {
        R = parseRotation(node->FirstChildElement("Rotate"), &myOk);
        if( !myOk ) {
          return Transformf();
        }
      }

      if( ok != nullptr ) {
        *ok = true;
      }

      return Transformf(R, t);
    }

    Vertex3f parseVertex(const tinyxml2::XMLElement *node, bool *ok)
    {
      return parseVector3D<Vertex3f>(node, "x", "y", "z", ok);
    }

  } // namespace priv

} // namespace rt
