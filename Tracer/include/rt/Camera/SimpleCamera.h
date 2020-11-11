/****************************************************************************
** Copyright (c) 2020, Carsten Schmidt. All rights reserved.
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

#ifndef SIMPLECAMERA_H
#define SIMPLECAMERA_H

#include "rt/Camera/ICamera.h"

namespace rt {

  class SimpleCamera : public ICamera {
  public:
    SimpleCamera();
    ~SimpleCamera();

    void setup(const real_T fov_rad);

    Image render(const std::size_t width, const std::size_t height,
                 std::size_t y0, std::size_t y1,
                 const Renderer& renderer, const std::size_t samples = 0) const;

  private:
    class Screen {
    public:
      Screen(const std::size_t width, const std::size_t height)
      {
        _width  = static_cast<real_T>(width);
        _height = static_cast<real_T>(height);
        _aspect = _width/_height;
        _x = _y = 0;
      }

      ~Screen()
      {
      }

      real_T aspect() const
      {
        return _aspect;
      }

      real_T width() const
      {
        return _width;
      }

      real_T height() const
      {
        return _height;
      }

      real_T x() const
      {
        return _x;
      }

      real_T y() const
      {
        return _y;
      }

      void setPos(const std::size_t x, const std::size_t y)
      {
        _x = static_cast<real_T>(x);
        _y = static_cast<real_T>(y);
      }

    private:
      Screen() = delete;
      real_T _aspect{}, _width{}, _height{}, _x{}, _y{};
    };

    Rayf ray(const Screen& screen, const real_T near, const bool random = false) const;
    real_T scale(const real_T in, const real_T s1, const real_T s2) const;

    real_T _fov_rad;
  };

} // namespace rt

#endif // SIMPLECAMERA_H
