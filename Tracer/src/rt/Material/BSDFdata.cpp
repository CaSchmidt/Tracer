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

#include "rt/Material/BSDFdata.h"

#include "rt/Object/SurfaceInfo.h"

namespace rt {

  BSDFdata::BSDFdata(const SurfaceInfo& info) noexcept
    : BSDFdata(info, ONE, SamplerPtr())
  {
  }

  BSDFdata::BSDFdata(const SurfaceInfo& info, const real_t etaA) noexcept
    : BSDFdata(info, etaA, SamplerPtr())
  {
  }

  BSDFdata::BSDFdata(const SurfaceInfo& info, const SamplerPtr& sampler) noexcept
    : BSDFdata(info, ONE, sampler)
  {
  }

  BSDFdata::BSDFdata(const SurfaceInfo& sinfo, const real_t etaA, const SamplerPtr& sampler) noexcept
    : etaA{etaA}
  {
    tex = sinfo.texCoord2D();

    if( sampler ) {
      xi =sampler->sample2D();
    }

    xfrmWS = n4::util::frameFromZ(sinfo.N);
    xfrmSW = xfrmWS.transpose();

    wo = sinfo.wo;
  }

} // namespace rt
