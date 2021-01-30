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

#include "rt/Material/TransparentMaterial.h"

namespace rt {

  ////// public //////////////////////////////////////////////////////////////

  TransparentMaterial::TransparentMaterial() noexcept
    : IMaterial()
  {
    _reflect  = std::make_unique<SpecularReflectionBRDF>();
    _transmit = std::make_unique<SpecularTransmissionBTDF>();
    setupPacks();
  }

  TransparentMaterial::~TransparentMaterial() noexcept
  {
  }

  MaterialPtr TransparentMaterial::copy() const
  {
    MaterialPtr result = create();
    TransparentMaterial *transparent = TRANSPARENT(result);
    transparent->setRefraction(refraction());
    return result;
  }

  BxDFpack TransparentMaterial::getBxDFs() const
  {
    return _bxdf;
  }

#if 0
  bool TransparentMaterial::isShadowCaster() const
  {
    return false;
  }
#endif

  void TransparentMaterial::setRefraction(const real_t eta)
  {
    _reflect->setRefraction(eta);
    _transmit->setRefraction(eta);
  }

  real_t TransparentMaterial::refraction() const
  {
    return _reflect->refraction();
  }

  MaterialPtr TransparentMaterial::create()
  {
    return std::make_unique<TransparentMaterial>();
  }

  ////// private /////////////////////////////////////////////////////////////

  void TransparentMaterial::setupPacks()
  {
    _bxdf.fill(nullptr);
    _bxdf[0] = _reflect.get();
    _bxdf[1] = _transmit.get();
  }

} // namespace rt
