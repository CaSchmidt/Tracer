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

#ifndef BSDFDATA_H
#define BSDFDATA_H

#include "rt/Sampler/ISampler.h"
#include "rt/Texture/TexCoord.h"

namespace rt {

  struct SurfaceInfo;

  struct BSDFdata {
    BSDFdata(const SurfaceInfo& info, const real_t etaA,
             const SamplerPtr& sampler = SamplerPtr()) noexcept;

    template<typename VecT>
    inline VecT toShading(const VecT& v) const
    {
      return xfrmSW*v;
    }

    template<typename VecT>
    inline VecT toWorld(const VecT& v) const
    {
      return xfrmWS*v;
    }

    real_t    etaA{1};
    TexCoord2D tex{};
    Direction   wo{}; // in Shading Coordinates
    Matrix  xfrmSW{}; // World -> Shading
    Matrix  xfrmWS{}; // Shading -> World
    Sample2D    xi{};

  private:
    BSDFdata() noexcept = delete;
  };

} // namespace rt

#endif // BSDFDATA_H
