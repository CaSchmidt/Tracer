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

#ifndef RENDERLOOP_H
#define RENDERLOOP_H

#include "Image.h"
#include "rt/Types.h"

namespace rt {

  template<typename RadianceFunc>
  void render_loop(Image& image, const size_t y0, const RadianceFunc& radiance)
  {
    constexpr real_t     GAMMA = 2.2f;
    constexpr real_t INV_GAMMA = 1.0f/GAMMA;
    constexpr real_t   MAX_RGB = 256.0 - 0x1p-7;

    const size_t y1 = y0 + image.height();
    for(size_t y = y0; y < y1; y++) {
      uint8_t *row = image.row(y - y0);
      for(size_t x = 0; x < image.width(); x++) {
        const Color Li = radiance(x, y);

        const real_t r = n4::pow(Li.r, INV_GAMMA);
        const real_t g = n4::pow(Li.g, INV_GAMMA);
        const real_t b = n4::pow(Li.b, INV_GAMMA);

        *row++ = static_cast<uint8_t>(std::clamp<real_t>(r, 0, 1)*MAX_RGB);
        *row++ = static_cast<uint8_t>(std::clamp<real_t>(g, 0, 1)*MAX_RGB);
        *row++ = static_cast<uint8_t>(std::clamp<real_t>(b, 0, 1)*MAX_RGB);
        *row++ = 0xFF;
      }
    }
  }

} // namespace rt

#endif // RENDERLOOP_H
