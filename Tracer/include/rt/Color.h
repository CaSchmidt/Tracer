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

#include "geom/VectorBase.h"
#include "rt/Types.h"

namespace rt {

  template<typename T>
  struct Color : public geom::VectorBase<T,Color<T>> {
    using base_type = geom::VectorBase<T,Color<T>>;

    // Construction & Assignment /////////////////////////////////////////////

    Color() noexcept = default;

    Color(const Color<T>&) noexcept = default;
    Color<T>& operator=(const Color<T>&) noexcept = default;

    Color(Color<T>&&) noexcept = default;
    Color<T>& operator=(Color<T>&&) noexcept = default;

    Color(const T& x, const T& y, const T& z) noexcept
      : base_type{x, y, z}
    {
    }

    Color(const base_type& other) noexcept
      : base_type{other}
    {
    }

    Color<T>& operator=(const base_type& other) noexcept
    {
      if( this != &other ) {
        base_type::operator=(other);
      }
      return *this;
    }

    Color(base_type&& other) noexcept
      : base_type{std::move(other)}
    {
    }

    Color<T>& operator=(base_type&& other) noexcept
    {
      if( this != &other ) {
        base_type::operator=(std::move(other));
      }
      return *this;
    }

    // Conversion ////////////////////////////////////////////////////////////

    constexpr uint8_t r8() const
    {
      return static_cast<uint8_t>(geom::clamp<T>(this->x, 0, 1)*T{255});
    }

    constexpr uint8_t g8() const
    {
      return static_cast<uint8_t>(geom::clamp<T>(this->y, 0, 1)*T{255});
    }

    constexpr uint8_t b8() const
    {
      return static_cast<uint8_t>(geom::clamp<T>(this->z, 0, 1)*T{255});
    }

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

  using Color3f = Color<real_T>;

} // namespace rt

#endif // COLOR_H
