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

#ifndef SCENELOADERSTRINGUTIL_H
#define SCENELOADERSTRINGUTIL_H

#include <cctype>

#include <algorithm>
#include <string>

namespace rt {

  namespace priv {

    template<typename CharT>
    inline bool isSpace(const CharT& ch)
    {
      return std::isspace(static_cast<int>(ch)) != 0;
    }

    template<typename CharT>
    inline std::size_t length(const CharT *s)
    {
      return s != nullptr
          ? std::char_traits<CharT>::length(s)
          : 0;
    }

    template<typename CharT>
    inline bool compare(const CharT *s1, const CharT *s2)
    {
      const std::size_t l1 = length(s1);
      const std::size_t l2 = length(s2);
      return l1 > 0  &&  l1 == l2
          ? std::char_traits<CharT>::compare(s1, s2, l1) == 0
          : false;
    }

    template<typename CharT>
    inline bool isHexPrefix(const CharT *s)
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

  } // namespace priv

} // namespace rt

#endif // SCENELOADERSTRINGUTIL_H
