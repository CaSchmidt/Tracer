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

#ifndef GEOM_H
#define GEOM_H

#include <cstdint>

#include <cs/NumericArray.h>

namespace geom {

  // Traits //////////////////////////////////////////////////////////////////

  template<typename value_T, std::size_t ROWS, std::size_t COLS>
  struct VertexTraits {
    using value_type = value_T;

    static constexpr std::size_t Columns = COLS;
    static constexpr std::size_t    Rows = ROWS;
    static constexpr std::size_t    Size = COLS*ROWS;
  };

  template<typename value_T, std::size_t ROWS, std::size_t COLS>
  struct NormalTraits {
    using value_type = value_T;

    static constexpr std::size_t Columns = COLS;
    static constexpr std::size_t    Rows = ROWS;
    static constexpr std::size_t    Size = COLS*ROWS;
  };

  // Types ///////////////////////////////////////////////////////////////////

  template<typename T>
  using Vertex = cs::Array<cs::Vector3Manip<cs::RowMajorPolicy<VertexTraits<T,3,1>>>>;

  template<typename T>
  using Normal = cs::Array<cs::Vector3Manip<cs::RowMajorPolicy<NormalTraits<T,3,1>>>>;

  template<typename T>
  using Matrix = cs::Array<cs::NoManipulator<cs::RowMajorPolicy<VertexTraits<T,3,3>>>>;

  // Helpers /////////////////////////////////////////////////////////////////

  template<typename T>
  inline auto identity()
  {
    return cs::identity<typename Matrix<T>::traits_type>();
  }

  template<typename T>
  inline auto rotateX(const T& angle)
  {
    return cs::rotateX<typename Matrix<T>::traits_type>(angle);
  }

  template<typename T>
  inline auto rotateY(const T& angle)
  {
    return cs::rotateY<typename Matrix<T>::traits_type>(angle);
  }

  template<typename T>
  inline auto rotateZ(const T& angle)
  {
    return cs::rotateZ<typename Matrix<T>::traits_type>(angle);
  }

  template<typename T>
  inline auto scale(const T& sx, const T& sy, const T& sz)
  {
    return cs::scale<typename Matrix<T>::traits_type>(sx, sy, sz);
  }

  template<typename T, typename EXPR>
  inline auto to_normal(const EXPR& expr)
  {
    return cs::array_cast<typename Normal<T>::traits_type>(expr);
  }

  template<typename T, typename EXPR>
  inline auto to_vertex(const EXPR& expr)
  {
    return cs::array_cast<typename Vertex<T>::traits_type>(expr);
  }

  // Operators ///////////////////////////////////////////////////////////////

  template<typename T>
  inline Normal<T> operator*(const Matrix<T>& M, const Normal<T>& n)
  {
    return to_normal<T>(M*to_vertex<T>(n));
  }

} // namespace geom

#endif // GEOM_H
