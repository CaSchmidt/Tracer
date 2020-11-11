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

#include <limits>
#include <type_traits>

#include "geom/Ray.h"
#include "geom/Transform.h"
#include "math/Constants.h"
#include "rt/Color.h"

namespace rt {

  ////// Floating-Point Type /////////////////////////////////////////////////

  template<typename T>
  inline constexpr bool if_real_v = std::is_floating_point_v<T>;

  template<typename T>
  using if_real_t = std::enable_if_t<if_real_v<T>,T>;

  using real_T = if_real_t<float>; // Implementation


  ////// Special Values //////////////////////////////////////////////////////

  inline constexpr real_T INF_REAL_T = std::numeric_limits<real_T>::infinity();

  inline constexpr real_T MAX_REAL_T = std::numeric_limits<real_T>::max();
  inline constexpr real_T MIN_REAL_T = std::numeric_limits<real_T>::lowest();

  inline constexpr real_T NAN_REAL_T = std::numeric_limits<real_T>::quiet_NaN();

  ////// Constants ///////////////////////////////////////////////////////////

  inline constexpr real_T  ZERO = static_cast<real_T>(0);
  inline constexpr real_T   ONE = static_cast<real_T>(1);
  inline constexpr real_T   TWO = static_cast<real_T>(2);
  inline constexpr real_T THREE = static_cast<real_T>(3);
  inline constexpr real_T  FOUR = static_cast<real_T>(4);
  inline constexpr real_T EIGHT = static_cast<real_T>(8);
  inline constexpr real_T   TEN = static_cast<real_T>(10);

  inline constexpr real_T ONE_HALF = static_cast<real_T>(0.5);

  inline constexpr real_T      PI      = math::PI<real_T>;
  inline constexpr real_T      PI_HALF = math::PI_HALF<real_T>;
  inline constexpr real_T  TWO_PI      = math::TWO_PI<real_T>;

  inline constexpr real_T TRACE_BIAS = static_cast<real_T>((1.0/1024.0)/10.0);

  inline constexpr bool NO_SHADOW_RAY = false;
  inline constexpr bool    SHADOW_RAY = true;

  ////// Spatial Types ///////////////////////////////////////////////////////

  using    Color3f = Color<real_T>;
  using   Matrix3f = geom::Matrix<real_T>;
  using   Normal3f = geom::Normal<real_T>;
  using       Rayf = geom::Ray<real_T>;
  using Transformf = geom::Transform<real_T>;
  using   Vertex3f = geom::Vertex<real_T>;

  template<typename EXPR>
  inline auto to_normal(const EXPR& expr)
  {
    return cs::array_cast<Normal3f::traits_type>(expr);
  }

  template<typename EXPR>
  inline auto to_vertex(const EXPR& expr)
  {
    return cs::array_cast<Vertex3f::traits_type>(expr);
  }

} // namespace rt

#endif // TYPES_H
