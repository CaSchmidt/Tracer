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

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <limits>

namespace math {

  template<typename T> inline constexpr T  Inf = std::numeric_limits<T>::infinity();
  template<typename T> inline constexpr T qNaN = std::numeric_limits<T>::quiet_NaN();

  template<typename T> inline constexpr T  ZERO = static_cast<T>(0);
  template<typename T> inline constexpr T   ONE = static_cast<T>(1);
  template<typename T> inline constexpr T   TWO = static_cast<T>(2);
  template<typename T> inline constexpr T THREE = static_cast<T>(3);
  template<typename T> inline constexpr T  FOUR = static_cast<T>(4);

  template<typename T> inline constexpr T ONE_HALF    = static_cast<T>(0.5);
  template<typename T> inline constexpr T ONE_QUARTER = static_cast<T>(0.25);

#ifndef M_PI
# define M_PI  3.14159265358979323846
#endif

  template<typename T> inline constexpr T     PI      = static_cast<T>(M_PI);
  template<typename T> inline constexpr T     PI_HALF = static_cast<T>(M_PI/2.0);
  template<typename T> inline constexpr T TWO_PI      = static_cast<T>(2.0*M_PI);

} // namespace math

#endif // CONSTANTS_H
