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

#include "geom/Transform.h"
#include "math/Constants.h"

namespace rt {

  ////// Types ///////////////////////////////////////////////////////////////

  using geom::real_t;
  using geom::size_t;

  using uint_t = unsigned int;

  using Color = n4::Color3f;
  using geom::Direction;
  using geom::Matrix;
  using geom::Normal;
  using geom::Vertex;

  using geom::Bounds;
  using geom::Ray;
  using geom::Transform;

  using geom::Math;

  ////// Special Values //////////////////////////////////////////////////////

  inline constexpr real_t INF_REAL_T = math::Inf<real_t>;

  inline constexpr real_t MAX_REAL_T = math::Max<real_t>;
  inline constexpr real_t MIN_REAL_T = math::Min<real_t>;

  inline constexpr real_t NAN_REAL_T = math::qNaN<real_t>;

  ////// Constants ///////////////////////////////////////////////////////////

  inline constexpr real_t ZERO = 0;
  inline constexpr real_t  ONE = 1;
  inline constexpr real_t  TWO = 2;

  inline constexpr real_t ONE_HALF = static_cast<real_t>(0.5);

  inline constexpr real_t      PI         = math::PI<real_t>;
  inline constexpr real_t      PI_HALF    = math::PI_HALF<real_t>;
  inline constexpr real_t      PI_QUARTER = math::PI_QUARTER<real_t>;
  inline constexpr real_t  TWO_PI         = math::TWO_PI<real_t>;
  inline constexpr real_t FOUR_PI         = math::FOUR_PI<real_t>;

  inline constexpr real_t EPSILON0_BASE = 0x1p-13;
  inline constexpr real_t   SHADOW_BIAS = 0x1p-10;

} // namespace rt

#endif // TYPES_H
