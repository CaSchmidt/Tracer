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

  CheckedTexture::CheckedTexture(const Color3f& colorA, const Color3f& colorB,
                                 const real_T scaleS, const real_T scaleT) noexcept
    : _colorA{colorA}
    , _colorB{colorB}
    , _scaleS{scaleS}
    , _scaleT{scaleT}
  {
  }

  CheckedTexture::~CheckedTexture()
  {
  }

  Color3f CheckedTexture::lookup(const real_T s, const real_T t) const
  {
    const bool a = std::fmod(s*_scaleS, 1) > 0.5;
    const bool b = std::fmod(t*_scaleT, 1) > 0.5;
    return math::XOR(a, b)
        ? _colorA
        : _colorB;
  }

  TexturePtr CheckedTexture::create(const Color3f& colorA, const Color3f& colorB,
                                    const real_T scaleS, const real_T scaleT)
  {
    return std::make_unique<CheckedTexture>(colorA, colorB, scaleS, scaleT);
  }

} // namespace rt
