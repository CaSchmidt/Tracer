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

#pragma once

#include <cs/Math/Math.h>

#include <N4/N4.h>

namespace geom {

  // Types ///////////////////////////////////////////////////////////////////

  using n4::real_t;

  using n4::size_t;

  struct DirectionTraits {
    static constexpr bool have_w = false;
  };

  using Direction = n4::Vector4f<DirectionTraits,n4::Normal3fData>;
  using    Matrix = n4::Matrix4f;
  using    Normal = n4::Normal3f;
  using    Vertex = n4::Vertex4f;

  using Math = cs::Math<real_t>;

  // Convenience Functions ///////////////////////////////////////////////////

  template<typename traits1_T, typename ARG1, typename traits2_T, typename ARG2>
  inline real_t absDot(const n4::ExprBase<traits1_T,ARG1>& arg1,
                       const n4::ExprBase<traits2_T,ARG2>& arg2)
  {
    return Math::abs(n4::dot(arg1, n4::expr_cast<traits1_T,traits2_T,ARG2>(arg2)));
  }

  template<typename traits1_T, typename ARG1, typename traits2_T, typename ARG2>
  inline real_t dot1(const n4::ExprBase<traits1_T,ARG1>& arg1,
                     const n4::ExprBase<traits2_T,ARG2>& arg2)
  {
    return std::max<real_t>(0, n4::dot(arg1, n4::expr_cast<traits1_T,traits2_T,ARG2>(arg2)));
  }

  template<typename traits1_T, typename ARG1, typename traits2_T, typename ARG2>
  inline bool isSameHemisphere(const n4::ExprBase<traits1_T,ARG1>& arg1,
                               const n4::ExprBase<traits2_T,ARG2>& arg2)
  {
    return n4::dot(arg1, n4::expr_cast<traits1_T,traits2_T,ARG2>(arg2)) >= real_t{0};
  }

  inline Direction spherical(const real_t sinTheta, const real_t cosTheta, const real_t phi)
  {
    return {sinTheta*Math::cos(phi), sinTheta*Math::sin(phi), cosTheta};
  }

  // Helpers /////////////////////////////////////////////////////////////////

  template<typename traits_T, typename EXPR>
  inline auto to_direction(const n4::ExprBase<traits_T,EXPR>& expr)
  {
    return n4::expr_cast<Direction::traits_type,traits_T,EXPR>(expr);
  }

  template<typename traits_T, typename EXPR>
  inline auto to_normal(const n4::ExprBase<traits_T,EXPR>& expr)
  {
    return n4::expr_cast<Normal::traits_type,traits_T,EXPR>(expr);
  }

  template<typename traits_T, typename EXPR>
  inline auto to_vertex(const n4::ExprBase<traits_T,EXPR>& expr)
  {
    return n4::expr_cast<Vertex::traits_type,traits_T,EXPR>(expr);
  }

} // namespace geom
