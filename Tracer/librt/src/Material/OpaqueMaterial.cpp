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

#include "rt/Material/OpaqueMaterial.h"

#include "rt/BxDF/LambertianBRDF.h"
#include "rt/BxDF/PhongBRDF.h"

namespace rt {

  ////// Constants ///////////////////////////////////////////////////////////

  inline constexpr size_t DIFF = 0; // LambertianBRDF
  inline constexpr size_t SPEC = 1; // PhongBRDF

  ////// public //////////////////////////////////////////////////////////////

  OpaqueMaterial::OpaqueMaterial() noexcept
    : IMaterial()
  {
    bsdf()->add(new LambertianBRDF()); // #0
    bsdf()->add(new PhongBRDF());      // #1
  }

  OpaqueMaterial::~OpaqueMaterial() noexcept
  {
  }

  MaterialPtr OpaqueMaterial::copy() const
  {
    MaterialPtr result = create();
    OpaqueMaterial *opaque = OPAQUE(result);
    if( _diffTex ) {
      opaque->setDiffuse(_diffTex->copy());
    }
    opaque->setShininess(shininess());
    if( _specTex ) {
      opaque->setSpecular(_specTex->copy());
    }
    return result;
  }

  bool OpaqueMaterial::haveTexture(const size_t i) const
  {
    return (i == DIFF  &&  _diffTex)  ||  (i == SPEC  &&  _specTex);
  }

  Color OpaqueMaterial::textureLookup(const size_t i, const TexCoord2D& tex) const
  {
    if(        i == DIFF ) {
      return _diffTex->lookup(tex);
    } else if( i == SPEC ) {
      return _specTex->lookup(tex);
    }
    return Color();
  }

  bool OpaqueMaterial::isSpecular() const
  {
    return bsdf()->asBxDF<PhongBRDF>(SPEC)->shininess() >= ONE  &&  _specTex;
  }

  void OpaqueMaterial::setDiffuse(TexturePtr& tex)
  {
    _diffTex = std::move(tex);
  }

  void OpaqueMaterial::setDiffuse(TexturePtr&& tex)
  {
    _diffTex = std::move(tex);
  }

  void OpaqueMaterial::setShininess(const real_t spec)
  {
    bsdf()->asBxDF<PhongBRDF>(SPEC)->setShininess(spec);
  }

  real_t OpaqueMaterial::shininess() const
  {
    return bsdf()->asBxDF<PhongBRDF>(SPEC)->shininess();
  }

  void OpaqueMaterial::setSpecular(TexturePtr& tex)
  {
    _specTex = std::move(tex);
  }

  void OpaqueMaterial::setSpecular(TexturePtr&& tex)
  {
    _specTex = std::move(tex);
  }

  MaterialPtr OpaqueMaterial::create()
  {
    return std::make_unique<OpaqueMaterial>();
  }

} // namespace rt
