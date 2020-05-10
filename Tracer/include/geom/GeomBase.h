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

#ifndef GEOMBASE_H
#define GEOMBASE_H

#include <cs/Array.h>

namespace geom {

  template<typename T>
  class ValueProperty {
  public:
    using value_type = T;

    ValueProperty(value_type *data) noexcept
      : _data{data}
    {
    }

    ~ValueProperty() noexcept = default;

    constexpr operator value_type() const
    {
      return *_data;
    }

    inline value_type operator=(const value_type& value)
    {
      *_data = value;
      return *_data;
    }

  private:
    ValueProperty() noexcept = delete;

    value_type *_data{nullptr};
  };

  template<typename policy_T>
  class VectorBase : public cs::Array<policy_T> {
  public:
    using base_type = cs::Array<policy_T>;
    using typename base_type::size_type;
    using typename base_type::traits_type;
    using typename base_type::value_type;

    ~VectorBase() noexcept = default;

    // Copy Assignment ///////////////////////////////////////////////////////

    VectorBase(const VectorBase<policy_T>& other) noexcept
      : base_type(other)
      , x(this->_data + 0)
      , y(this->_data + 1)
      , z(this->_data + 2)
    {
    }

    VectorBase<policy_T>& operator=(const VectorBase<policy_T>& other) noexcept
    {
      if( this != &other ) {
        base_type::operator=(other);
      }
      return *this;
    }

    // Move Assignment ///////////////////////////////////////////////////////

    VectorBase(VectorBase<policy_T>&& other) noexcept
      : base_type(std::move(other))
      , x(this->_data + 0)
      , y(this->_data + 1)
      , z(this->_data + 2)
    {
    }

    VectorBase<policy_T>& operator=(VectorBase<policy_T>&& other) noexcept
    {
      if( this != &other ) {
        base_type::operator=(std::move(other));
      }
      return *this;
    }

    // Scalar Assignment /////////////////////////////////////////////////////

    VectorBase(const value_type& value = value_type{0}) noexcept
      : base_type(value)
      , x(this->_data + 0)
      , y(this->_data + 1)
      , z(this->_data + 2)
    {
    }

    VectorBase<policy_T>& operator=(const value_type& value) noexcept
    {
      base_type::operator=(value);
      return *this;
    }

    // List Assignment ///////////////////////////////////////////////////////

    VectorBase(const std::initializer_list<value_type>& list) noexcept
      : base_type(list)
      , x(this->_data + 0)
      , y(this->_data + 1)
      , z(this->_data + 2)
    {
    }

    VectorBase<policy_T>& operator=(const std::initializer_list<value_type>& list) noexcept
    {
      base_type::operator=(list);
      return *this;
    }

    // Expression Assignment /////////////////////////////////////////////////

    template<typename EXPR>
    VectorBase(const cs::ExprBase<traits_type,EXPR>& expr)
      : base_type(expr)
      , x(this->_data + 0)
      , y(this->_data + 1)
      , z(this->_data + 2)
    {
    }

    template<typename EXPR>
    VectorBase<policy_T>& operator=(const cs::ExprBase<traits_type,EXPR>& expr)
    {
      base_type::operator=(expr);
      return *this;
    }

    // Data //////////////////////////////////////////////////////////////////

    ValueProperty<value_type> x, y, z;
  };

} // namespace geom

#endif // GEOMBASE_H
