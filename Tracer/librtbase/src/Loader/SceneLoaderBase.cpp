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
      result(0) = parseNodeAsFloat<value_type>(node->FirstChildElement(id0), &myOk);
      if( !myOk ) {
        return T();
      }

      myOk = false;
      result(1) = parseNodeAsFloat<value_type>(node->FirstChildElement(id1), &myOk);
      if( !myOk ) {
        return T();
      }

      myOk = false;
      result(2) = parseNodeAsFloat<value_type>(node->FirstChildElement(id2), &myOk);
      if( !myOk ) {
        return T();
      }

      if( ok != nullptr ) {
        *ok = true;
      }

      return result;
    }

    ////// Interface /////////////////////////////////////////////////////////

    real_t parseAngle(const tinyxml2::XMLElement *node, bool *ok)
    {
      return math::radian<real_t>(parseNodeAsFloat<real_t>(node, ok));
    }

    Color parseColor(const tinyxml2::XMLElement *node, bool *ok)
    {
      const Color result = parseVector3D<Color>(node, "r", "g", "b", ok);
      return n4::clamp(result, 0, 1);
    }

    Direction parseDirection(const tinyxml2::XMLElement *node, bool *ok)
    {
      return n4::normalize(parseVector3D<Direction>(node, "x", "y", "z", ok));
    }

    Normal parseNormal(const tinyxml2::XMLElement *node, bool *ok)
    {
      return n4::normalize(parseVector3D<Normal>(node, "x", "y", "z", ok));
    }

    real_t parseReal(const tinyxml2::XMLElement *node, bool *ok)
    {
      return parseNodeAsFloat<real_t>(node, ok);
    }

    Matrix parseRotation(const tinyxml2::XMLElement *node, bool *ok)
    {
      using MyElem = tinyxml2::XMLElement;

      if( ok != nullptr ) {
        *ok = false;
      }

      if( node == nullptr ) {
        return Matrix();
      }

      bool myOk = false;

      Matrix Rx;
      if( const MyElem *elem = node->FirstChildElement("rx"); elem != nullptr ) {
        const real_t rx = parseAngle(elem, &myOk);
        if( !myOk ) {
          return Matrix();
        }
        Rx = n4::rotateX(rx);
      } else if( const MyElem *elem = node->FirstChildElement("rxByPI2"); elem != nullptr ) {
        const signed int rx = parseSignedInt(elem, &myOk);
        if( !myOk ) {
          return Matrix();
        }
        Rx = n4::rotateXbyPI2(rx);
      } else {
        Rx = n4::identity();
      }

      Matrix Ry;
      if( const MyElem *elem = node->FirstChildElement("ry"); elem != nullptr ) {
        const real_t ry = parseAngle(elem, &myOk);
        if( !myOk ) {
          return Matrix();
        }
        Ry = n4::rotateY(ry);
      } else if( const MyElem *elem = node->FirstChildElement("ryByPI2"); elem != nullptr ) {
        const signed int ry = parseSignedInt(elem, &myOk);
        if( !myOk ) {
          return Matrix();
        }
        Ry = n4::rotateYbyPI2(ry);
      } else {
        Ry = n4::identity();
      }

      Matrix Rz;
      if( const MyElem *elem = node->FirstChildElement("rz"); elem != nullptr ) {
        const real_t rz = parseAngle(elem, &myOk);
        if( !myOk ) {
          return Matrix();
        }
        Rz = n4::rotateZ(rz);
      } else if( const MyElem *elem = node->FirstChildElement("rzByPI2"); elem != nullptr ) {
        const signed int rz = parseSignedInt(elem, &myOk);
        if( !myOk ) {
          return Matrix();
        }
        Rz = n4::rotateZbyPI2(rz);
      } else {
        Rz = n4::identity();
      }

      if( ok != nullptr ) {
        *ok = true;
      }

      return Rz*Ry*Rx;
    }

    signed int parseSignedInt(const tinyxml2::XMLElement *node, bool *ok)
    {
      return parseNodeAsInt<signed int>(node, ok);
    }

    size_t parseSize(const tinyxml2::XMLElement *node, bool *ok)
    {
      return parseNodeAsInt<size_t>(node, ok);
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

    Transform parseTransform(const tinyxml2::XMLElement *node, bool *ok)
    {
      using MyElem = tinyxml2::XMLElement;

      if( ok != nullptr ) {
        *ok = false;
      }

      if( node == nullptr ) {
        return Transform();
      }

      bool myOk = false;

      Vertex t = 0;
      if( const MyElem *elem = node->FirstChildElement("Translate"); elem != nullptr ) {
        t = parseVertex(elem, &myOk);
        if( !myOk ) {
          return Transform();
        }
      }

      Matrix R = n4::identity();
      if( const MyElem *elem = node->FirstChildElement("Rotate"); elem != nullptr ) {
        R = parseRotation(elem, &myOk);
        if( !myOk ) {
          return Transform();
        }
      }

      if( ok != nullptr ) {
        *ok = true;
      }

      return Transform(n4::translate(t.x, t.y, t.z)*R);
    }

    Vertex parseVertex(const tinyxml2::XMLElement *node, bool *ok)
    {
      return parseVector3D<Vertex>(node, "x", "y", "z", ok);
    }

  } // namespace priv

} // namespace rt
