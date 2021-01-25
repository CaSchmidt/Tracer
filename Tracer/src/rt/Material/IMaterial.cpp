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

#include "rt/Material/IMaterial.h"

#if 0
#include "rt/Material/MirrorMaterial.h"
#include "rt/Material/OpaqueMaterial.h"
#include "rt/Material/TransparentMaterial.h"
#endif

namespace rt {

  IMaterial::~IMaterial() noexcept
  {
  }

  bool IMaterial::haveTexture(const size_t /*i*/) const
  {
    return false;
  }

  Color IMaterial::textureLookup(const size_t /*i*/, const real_t /*s*/, const real_t /*t*/) const
  {
    return Color{1, 1, 1};
  }

  bool IMaterial::isShadowCaster() const
  {
    const BxDFpack bxdfs = getBxDFs();
    for(const IBxDF *bxdf : bxdfs) {
      if( bxdf != nullptr  &&  bxdf->isShadowCaster() ) {
        return true;
      }
    }
    return false;
  }

#if 0
  bool IMaterial::isMirror() const
  {
    return dynamic_cast<const MirrorMaterial*>(this) != nullptr;
  }

  MirrorMaterial *IMaterial::mirror()
  {
    return dynamic_cast<MirrorMaterial*>(this);
  }

  const MirrorMaterial *IMaterial::mirror() const
  {
    return dynamic_cast<const MirrorMaterial*>(this);
  }

  bool IMaterial::isOpaque() const
  {
    return dynamic_cast<const OpaqueMaterial*>(this) != nullptr;
  }

  OpaqueMaterial *IMaterial::opaque()
  {
    return dynamic_cast<OpaqueMaterial*>(this);
  }

  const OpaqueMaterial *IMaterial::opaque() const
  {
    return dynamic_cast<const OpaqueMaterial*>(this);
  }

  bool IMaterial::isTransparent() const
  {
    return dynamic_cast<const TransparentMaterial*>(this) != nullptr;
  }

  TransparentMaterial *IMaterial::transparent()
  {
    return dynamic_cast<TransparentMaterial*>(this);
  }

  const TransparentMaterial *IMaterial::transparent() const
  {
    return dynamic_cast<const TransparentMaterial*>(this);
  }
#endif

} // namespace rt
