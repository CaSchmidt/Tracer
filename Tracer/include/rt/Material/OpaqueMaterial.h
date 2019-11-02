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

#ifndef OPAQUEMATERIAL_H
#define OPAQUEMATERIAL_H

#include "rt/Material/IMaterial.h"
#include "rt/Texture/ITexture.h"

namespace rt {

  class OpaqueMaterial : public IMaterial {
  public:
    OpaqueMaterial();
    ~OpaqueMaterial();

    bool isShadowCaster() const;

    bool isSpecular() const;

    void setDiffuse(TexturePtr& tex);
    void setDiffuse(TexturePtr&& tex);
    Color3f diffuse(const real_T s, const real_T t) const;

    void setShininess(const real_T mSpec);
    real_T shininess() const;

    void setSpecular(TexturePtr& tex);
    void setSpecular(TexturePtr&& tex);
    Color3f specular(const real_T s, const real_T t) const;

    static MaterialPtr create();

  private:
    TexturePtr _diffTex{};
    real_T     _shininess{};
    TexturePtr _specTex{};
  };

} // namespace rt

#endif // OPAQUEMATERIAL_H
