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

#ifndef COLOR_H
#define COLOR_H

#include <cstdint>

#include <cs/Array.h>
#include <cs/ArrayPolicy.h>
#include <cs/Manipulator.h>
#include <cs/Math.h>

namespace rt {

  template<typename value_T, typename size_T, size_T ROWS, size_T COLS>
  struct ColorTraits {
    using  size_type = size_T;
    using value_type = value_T;

    static constexpr size_type Columns = COLS;
    static constexpr size_type    Rows = ROWS;
    static constexpr size_type    Size = COLS*ROWS;
  };

  template<typename T>
  class ColorProperty {
  public:
    using value_type = T;

    ColorProperty(value_type *data) noexcept
      : _data{data}
    {
    }

    ~ColorProperty() noexcept = default;

    constexpr operator value_type() const
    {
      return *_data;
    }

    inline value_type operator=(const value_type& value)
    {
      *_data = value;
      return *_data;
    }

    constexpr operator uint8_t() const
    {
      return static_cast<uint8_t>(csClamp(*_data, value_type{0}, value_type{1})*value_type{255});
    }

  private:
    ColorProperty() noexcept = delete;

    value_type *_data;
  };

  template<typename T>
  class Color : public cs::Array<cs::NoManipulator<cs::RowMajorPolicy<ColorTraits<T,uint8_t,3,1>>>> {
  public:
    using base_type = cs::Array<cs::NoManipulator<cs::RowMajorPolicy<ColorTraits<T,uint8_t,3,1>>>>;
    using typename base_type::size_type;
    using typename base_type::traits_type;
    using typename base_type::value_type;

    ~Color() noexcept = default;

    // Copy Assignment ///////////////////////////////////////////////////////

    Color(const Color<T>& other) noexcept
      : base_type(other)
      , r(this->_data + 0)
      , g(this->_data + 1)
      , b(this->_data + 2)
    {
    }

    Color<T>& operator=(const Color<T>& other) noexcept
    {
      if( this != &other ) {
        base_type::operator=(other);
      }
      return *this;
    }

    // Move Assignment ///////////////////////////////////////////////////////

    Color(Color<T>&& other) noexcept
      : base_type(std::move(other))
      , r(this->_data + 0)
      , g(this->_data + 1)
      , b(this->_data + 2)
    {
    }

    Color<T>& operator=(Color<T>&& other) noexcept
    {
      if( this != &other ) {
        base_type::operator=(std::move(other));
      }
      return *this;
    }

    // Scalar Assignment /////////////////////////////////////////////////////

    Color(const value_type& value = value_type{0}) noexcept
      : base_type(value)
      , r(this->_data + 0)
      , g(this->_data + 1)
      , b(this->_data + 2)
    {
    }

    Color<T>& operator=(const value_type& value) noexcept
    {
      base_type::operator=(value);
      return *this;
    }

    // List Assignment ///////////////////////////////////////////////////////

    Color(const std::initializer_list<value_type>& list) noexcept
      : base_type(list)
      , r(this->_data + 0)
      , g(this->_data + 1)
      , b(this->_data + 2)
    {
    }

    Color<T>& operator=(const std::initializer_list<value_type>& list) noexcept
    {
      base_type::operator=(list);
      return *this;
    }

    // Expression Assignment /////////////////////////////////////////////////

    template<typename EXPR>
    Color(const cs::ExprBase<traits_type,EXPR>& expr)
      : base_type(expr)
      , r(this->_data + 0)
      , g(this->_data + 1)
      , b(this->_data + 2)
    {
    }

    template<typename EXPR>
    Color<T>& operator=(const cs::ExprBase<traits_type,EXPR>& expr)
    {
      base_type::operator=(expr);
      return *this;
    }

    // Data //////////////////////////////////////////////////////////////////

    ColorProperty<value_type> r, g, b;

    // Constants /////////////////////////////////////////////////////////////

    static constexpr Color<T> black()
    {
      return Color<T>{0, 0, 0};
    }

    static constexpr Color<T> white()
    {
      return Color<T>{1, 1, 1};
    }

    static constexpr Color<T> red()
    {
      return Color<T>{1, 0, 0};
    }

    static constexpr Color<T> green()
    {
      return Color<T>{0, 1, 0};
    }

    static constexpr Color<T> blue()
    {
      return Color<T>{0, 0, 1};
    }

    static constexpr Color<T> cyan()
    {
      return Color<T>{0, 1, 1};
    }

    static constexpr Color<T> magenta()
    {
      return Color<T>{1, 0, 1};
    }

    static constexpr Color<T> yellow()
    {
      return Color<T>{1, 1, 0};
    }

    static constexpr Color<T> orange()
    {
      return Color<T>{1,  0.5, 0};
    }

    static constexpr Color<T> indigo()
    {
      return Color<T>{0.25, 0, 1};
    }

    static constexpr Color<T> violet()
    {
      return Color<T>{0.5,  0, 1};
    }
  };

} // namespace rt

#endif // COLOR_H
