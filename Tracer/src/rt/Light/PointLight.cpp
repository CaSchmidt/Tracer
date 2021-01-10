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

#include "rt/Light/PointLight.h"

namespace rt {

  ////// public //////////////////////////////////////////////////////////////

  PointLight::PointLight(const Color& IL, const Vertex& PL) noexcept
    : _IL{IL}
    , _PL{PL}
  {
  }

  PointLight::~PointLight() noexcept
  {
  }

  LightInfo PointLight::info(const Vertex& P) const
  {
    LightInfo i;
    i.l  = geom::to_direction(n4::direction(P, _PL));
    i.r  = n4::distance(P, _PL);
    i.EL = _IL*attenuation(i.r);
    return i;
  }

  LightSourcePtr PointLight::create(const Color& IL, const Vertex& PL)
  {
    return std::make_unique<PointLight>(IL, PL);
  }

  ////// private /////////////////////////////////////////////////////////////

  real_t PointLight::attenuation(const real_t r) const
  {
    const real_t rL = std::max<real_t>(r, ONE);
    return ONE/rL/rL;
  }

} // namespace rt
