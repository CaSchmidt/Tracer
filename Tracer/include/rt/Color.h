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
  using Color = cs::Array<cs::Color3Manip<cs::RowMajorPolicy<ColorTraits<T,uint8_t,3,1>>>>;

  template<typename T>
  struct Colors {
    static constexpr auto black()
    {
      return Color<T>{0, 0, 0};
    }

    static constexpr auto white()
    {
      return Color<T>{1, 1, 1};
    }

    static constexpr auto red()
    {
      return Color<T>{1, 0, 0};
    }

    static constexpr auto green()
    {
      return Color<T>{0, 1, 0};
    }

    static constexpr auto blue()
    {
      return Color<T>{0, 0, 1};
    }

    static constexpr auto cyan()
    {
      return Color<T>{0, 1, 1};
    }

    static constexpr auto magenta()
    {
      return Color<T>{1, 0, 1};
    }

    static constexpr auto yellow()
    {
      return Color<T>{1, 1, 0};
    }

    static constexpr auto orange()
    {
      return Color<T>{1,  0.5, 0};
    }

    static constexpr auto indigo()
    {
      return Color<T>{0.25, 0, 1};
    }

    static constexpr auto violet()
    {
      return Color<T>{0.5,  0, 1};
    }
  };

} // namespace rt

#endif // COLOR_H
