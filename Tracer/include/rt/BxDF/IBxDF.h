/****************************************************************************
** Copyright (c) 2021, Carsten Schmidt. All rights reserved.
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

#ifndef IBXDF_H
#define IBXDF_H

#include <array>

#include "rt/Types.h"

namespace rt {

  struct SurfaceInfo;

  struct BxDFdata {
    BxDFdata(const Ray& ray, const SurfaceInfo& info, const real_t etaI) noexcept;

    real_t   etai{1};
    Direction  wo{}; // in Shading Coordinates
    Matrix xfrmSW{}; // World-to-Shading
    Matrix xfrmWS{}; // Shading-to-World
  };

  class IBxDF {
  public:
    enum Type : unsigned int {
      Invalid      = 0,
      Reflection   = 1 << 0,
      Transmission = 1 << 1,
      Diffuse      = 1 << 2,
      Glossy       = 1 << 3,
      Specular     = 1 << 4,
      All          = 0x1F
    };

    IBxDF(const Type type) noexcept;
    virtual ~IBxDF();

    Type type() const;
    bool isType(const Type t) const;

    virtual bool isShadowCaster() const = 0;

    virtual Color eval(const Direction& wo, const Direction& wi) const = 0;
    virtual Color sample(const BxDFdata& input, Direction& wi) const;

  private:
    Type _type;
  };

  using BxDFpack = std::array<const IBxDF*,2>;

} // namespace rt

#endif // IBXDF_H
