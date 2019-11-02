/****************************************************************************
** Copyright (c) 2019, Carsten Schmidt. All rights reserved.
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

#ifndef TYPES_H
#define TYPES_H

#include <cstdint>

#include <numeric>

#include "geom/Ray.h"
#include "geom/Transform.h"

namespace rt {

  ////// Floating-Point Type /////////////////////////////////////////////////

  template<typename T>
  using if_real_T = std::enable_if_t<std::is_floating_point_v<T>,T>;

  using real_T = if_real_T<float>; // Implementation

  ////// Other Types /////////////////////////////////////////////////////////

  template<typename T>
  using if_dim_T = std::enable_if_t<std::is_unsigned_v<T>,T>;

  using dim_T = if_dim_T<uint16_t>; // Implementation

  template<typename T>
  using if_enum_T = std::enable_if_t<std::is_unsigned_v<T>,T>;

  using enum_T = if_enum_T<unsigned int>; // Implementation

  ////// Special Values //////////////////////////////////////////////////////

  constexpr real_T INF_REAL_T = std::numeric_limits<real_T>::infinity();

  constexpr real_T MAX_REAL_T = std::numeric_limits<real_T>::max();
  constexpr real_T MIN_REAL_T = std::numeric_limits<real_T>::lowest();

  constexpr real_T NAN_REAL_T = std::numeric_limits<real_T>::quiet_NaN();

  ////// Constants ///////////////////////////////////////////////////////////

#ifndef M_PI
# define M_PI  3.14159265358979323846
#endif

  constexpr real_T  ZERO = static_cast<real_T>(0);
  constexpr real_T   ONE = static_cast<real_T>(1);
  constexpr real_T   TWO = static_cast<real_T>(2);
  constexpr real_T THREE = static_cast<real_T>(3);
  constexpr real_T  FOUR = static_cast<real_T>(4);
  constexpr real_T EIGHT = static_cast<real_T>(8);
  constexpr real_T   TEN = static_cast<real_T>(10);

  constexpr real_T ONE_HALF = static_cast<real_T>(0.5);

  constexpr real_T      PI = static_cast<real_T>(M_PI);
  constexpr real_T PI_HALF = static_cast<real_T>(M_PI/2.0);

  constexpr real_T TRACE_BIAS = static_cast<real_T>((1.0/1024.0)/10.0);

  constexpr bool NO_SHADOW_RAY = false;
  constexpr bool    SHADOW_RAY = true;

  ////// Spatial Types ///////////////////////////////////////////////////////

  using   Matrix3f = geom::Matrix<real_T>;
  using   Normal3f = geom::Normal<real_T>;
  using       Rayf = geom::Ray<real_T>;
  using Transformf = geom::Transform<real_T>;
  using   Vertex3f = geom::Vertex<real_T>;

} // namespace rt

#endif // TYPES_H
