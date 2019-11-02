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

namespace rt {

  OpaqueMaterial::OpaqueMaterial()
    : IMaterial()
  {
  }

  OpaqueMaterial::~OpaqueMaterial()
  {
  }

  bool OpaqueMaterial::isShadowCaster() const
  {
    return true;
  }

  bool OpaqueMaterial::isSpecular() const
  {
    return _shininess >= 1  &&  _specTex;
  }

  void OpaqueMaterial::setDiffuse(TexturePtr& tex)
  {
    _diffTex = std::move(tex);
  }

  void OpaqueMaterial::setDiffuse(TexturePtr&& tex)
  {
    _diffTex = std::move(tex);
  }

  Color3f OpaqueMaterial::diffuse(const real_T s, const real_T t) const
  {
    return _diffTex
        ? _diffTex->lookup(s, t)/PI
        : Color3f::black();
  }

  void OpaqueMaterial::setShininess(const real_T mSpec)
  {
    _shininess = std::max<real_T>(mSpec, 0);
  }

  real_T OpaqueMaterial::shininess() const
  {
    return _shininess;
  }

  void OpaqueMaterial::setSpecular(TexturePtr& tex)
  {
    _specTex = std::move(tex);
  }

  void OpaqueMaterial::setSpecular(TexturePtr&& tex)
  {
    _specTex = std::move(tex);
  }

  Color3f OpaqueMaterial::specular(const real_T s, const real_T t) const
  {
    return isSpecular()
        ? _specTex->lookup(s, t)*(_shininess + EIGHT)/EIGHT/PI
        : Color3f::black();
  }

  MaterialPtr OpaqueMaterial::create()
  {
    return std::make_unique<OpaqueMaterial>();
  }

} // namespace rt
