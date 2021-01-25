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

#ifndef IMATERIAL_H
#define IMATERIAL_H

#include <memory>

#include "rt/BxDF/IBxDF.h"

namespace rt {

  class IBxDF;

#if 0
  class MirrorMaterial;
  class OpaqueMaterial;
  class TransparentMaterial;
#endif

  class IMaterial {
  public:
    virtual ~IMaterial() noexcept;

    virtual std::unique_ptr<IMaterial> copy() const = 0;

    virtual BxDFpack getBxDFs() const = 0;

    virtual bool haveTexture(const size_t i) const;

    virtual Color textureLookup(const size_t i, const real_t s, const real_t t) const;

    bool isShadowCaster() const;

#if 0
    bool isMirror() const;
    MirrorMaterial *mirror();
    const MirrorMaterial *mirror() const;

    bool isOpaque() const;
    OpaqueMaterial *opaque();
    const OpaqueMaterial *opaque() const;

    bool isTransparent() const;
    TransparentMaterial *transparent();
    const TransparentMaterial *transparent() const;
#endif
  };

  using MaterialPtr = std::unique_ptr<IMaterial>;

} // namespace rt

#endif // IMATERIAL_H
