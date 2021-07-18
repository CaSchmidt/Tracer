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

#include "rt/Renderer/BaseRenderer.h"

#include "rt/Material/BSDF.h"
#include "rt/Object/IObject.h"
#include "rt/Object/SurfaceInfo.h"

namespace rt {

  ////// public //////////////////////////////////////////////////////////////

  BaseRenderer::BaseRenderer(const RenderOptions& options) noexcept
    : IRenderer(options)
  {
  }

  BaseRenderer::~BaseRenderer() noexcept
  {
  }

  ////// protected ///////////////////////////////////////////////////////////

  Color BaseRenderer::specularReflectOrTransmit(const SurfaceInfo& ref, const ScenePtr& scene,
                                                const SamplerPtr& sampler, const uint_t depth,
                                                const bool is_transmit) const
  {
    const BSDF         *bsdf = ref->material()->bsdf();
    const IBxDF::Flags flags = is_transmit
        ? IBxDF::Flags(IBxDF::Specular | IBxDF::Transmission)
        : IBxDF::Flags(IBxDF::Specular | IBxDF::Reflection);

    real_t    pdf = 0;
    Direction  wi{};
    const Color         f = bsdf->sample(ref, &wi, sampler->sample2D(), &pdf, flags);
    const real_t absCosTi = geom::absDot(wi, ref.N);
    if( pdf <= ZERO  ||  absCosTi == ZERO  ||  f.isZero() ) {
      return Color();
    }

    const Color Li = radiance(ref.ray(wi), scene, sampler, depth + 1);
    return f*Li*absCosTi/pdf;
  }

} // namespace rt
