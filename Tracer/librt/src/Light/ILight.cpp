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

#include "rt/Light/ILight.h"

namespace rt {

  ////// public //////////////////////////////////////////////////////////////

  ILight::ILight(const Type type, const Transform& lightToWorld) noexcept
    : _type{type}
    , _xfrmWL{lightToWorld}
  {
    _xfrmLW = _xfrmWL.inverse();
  }

  ILight::~ILight() noexcept
  {
  }

  bool ILight::isDeltaLight() const
  {
    return _type == DeltaDirection  ||  _type == DeltaPosition;
  }

  size_t ILight::numSamples() const
  {
    return _numSamples;
  }

  void ILight::setNumSamples(const size_t numSamples)
  {
    _numSamples = numSamples > 1
        ? numSamples
        : 1;
  }

  real_t ILight::scale() const
  {
    return _scale;
  }

  void ILight::setScale(const real_t s)
  {
    _scale = s > ZERO
        ? s
        : 1;
  }

  ILight::Type ILight::type() const
  {
    return _type;
  }

} // namespace rt
