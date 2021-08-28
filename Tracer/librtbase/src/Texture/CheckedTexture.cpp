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

#include "rt/Texture/CheckedTexture.h"

#include "math/Logical.h"

namespace rt {

  CheckedTexture::CheckedTexture(const size_t id,
                                 const Color& colorA, const Color& colorB,
                                 const real_t scaleS, const real_t scaleT) noexcept
    : ITexture(id)
    , _scaleS{scaleS}
    , _scaleT{scaleT}
  {
    _colorA = n4::clamp(colorA, 0, 1);
    _colorB = n4::clamp(colorB, 0, 1);
  }

  CheckedTexture::~CheckedTexture() noexcept
  {
  }

  TexturePtr CheckedTexture::copy() const
  {
    return create(id(), _colorA, _colorB, _scaleS, _scaleT);
  }

  Color CheckedTexture::lookup(const TexCoord2D& tex) const
  {
    TEXCOORDS_2D(tex);
    const bool a = n4::remndr(s*_scaleS, ONE) > ONE_HALF;
    const bool b = n4::remndr(t*_scaleT, ONE) > ONE_HALF;
    return math::XOR(a, b)
        ? _colorA
        : _colorB;
  }

  TexturePtr CheckedTexture::create(const size_t id,
                                    const Color& colorA, const Color& colorB,
                                    const real_t scaleS, const real_t scaleT)
  {
    if( scaleS <= ZERO  ||  scaleT <= ZERO ) {
      return TexturePtr();
    }
    return std::make_unique<CheckedTexture>(id, colorA, colorB, scaleS, scaleT);
  }

} // namespace rt
