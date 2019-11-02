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

#ifndef VECTOR_H
#define VECTOR_H

#include "geom/VectorBase.h"

namespace geom {

  enum class VectorId : unsigned int {
    Vertex = 0,
    Normal = 1
  };

  template<typename T, VectorId ID>
  struct Vector : public VectorBase<T,Vector<T,ID>> {
    using base_type = VectorBase<T,Vector<T,ID>>;

    Vector() noexcept = default;

    Vector(const Vector<T,ID>&) noexcept = default;
    Vector<T,ID>& operator=(const Vector<T,ID>&) noexcept = default;

    Vector(Vector<T,ID>&&) noexcept = default;
    Vector<T,ID>& operator=(Vector<T,ID>&&) noexcept = default;

    Vector(const T& x, const T& y, const T& z) noexcept
      : base_type{x, y, z}
    {
    }

    Vector(const base_type& other) noexcept
      : base_type{other}
    {
    }

    Vector<T,ID>& operator=(const base_type& other) noexcept
    {
      if( this != &other ) {
        base_type::operator=(other);
      }
      return *this;
    }

    Vector(base_type&& other) noexcept
      : base_type{std::move(other)}
    {
    }

    Vector<T,ID>& operator=(base_type&& other) noexcept
    {
      if( this != &other ) {
        base_type::operator=(std::move(other));
      }
      return *this;
    }

    // Special Vectors ///////////////////////////////////////////////////////

    static constexpr Vector<T,ID> xAxis()
    {
      return Vector<T,ID>{1, 0, 0};
    }

    static constexpr Vector<T,ID> yAxis()
    {
      return Vector<T,ID>{0, 1, 0};
    }

    static constexpr Vector<T,ID> zAxis()
    {
      return Vector<T,ID>{0, 0, 1};
    }
  };

  template<typename T>
  using Vertex = Vector<T,VectorId::Vertex>;

  template<typename T>
  using Normal = Vector<T,VectorId::Normal>;

} // namespace geom

#endif // VECTOR_H
