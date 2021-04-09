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

#include "rt/Material/MatteMaterial.h"

#include "rt/BxDF/LambertianBRDF.h"

namespace rt {

  MatteMaterial::MatteMaterial() noexcept
    : IMaterial()
  {
    bsdf()->add(new LambertianBRDF());
  }

  MatteMaterial::~MatteMaterial() noexcept
  {
  }

  MaterialPtr MatteMaterial::copy() const
  {
    MaterialPtr result = create();
    MatteMaterial *matte = MATTE(result);
    matte->setColor(color());
    return result;
  }

  Color MatteMaterial::color() const
  {
    return bsdf()->asBxDF<LambertianBRDF>(0)->color();
  }

  void MatteMaterial::setColor(const Color& c)
  {
    bsdf()->asBxDF<LambertianBRDF>(0)->setColor(c);
  }

  MaterialPtr MatteMaterial::create()
  {
    return std::make_unique<MatteMaterial>();
  }

} // namespace rt
