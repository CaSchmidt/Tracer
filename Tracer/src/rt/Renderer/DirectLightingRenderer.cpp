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

#include "rt/Renderer/DirectLightingRenderer.h"

#include "rt/Material/BSDFdata.h"
#include "rt/Object/SurfaceInfo.h"
#include "rt/Renderer/RenderUtils.h"

namespace rt {

  ////// public //////////////////////////////////////////////////////////////

  DirectLightingRenderer::DirectLightingRenderer() noexcept
  {
  }

  DirectLightingRenderer::~DirectLightingRenderer() noexcept
  {
  }

  ////// private /////////////////////////////////////////////////////////////

  Color DirectLightingRenderer::radiance(const Ray& ray, const SamplerPtr& sampler,
                                         const unsigned int depth) const
  {
    const RenderOptions& options = DirectLightingRenderer::options();
    const Scene&           scene = DirectLightingRenderer::scene();

    SurfaceInfo surface;
    if( !scene.intersect(&surface, ray) ) {
      // NOTE: PBR3 uses an InfiniteAreaLight to compute background radiance.
      return options.backgroundColor;
    }

    Color Lo;

    Lo += Color(); // TODO: Account for emitted radiance when hitting an area light!

    if( scene.lights().size() > 0 ) {
      Lo += uniformSampleOneLight(surface, scene, sampler);
    }

    if( depth + 1 < options.maxDepth ) {
      const BSDFdata data(surface);
      Lo += specularReflectOrTransmit(data, surface, sampler, depth, false);
      Lo += specularReflectOrTransmit(data, surface, sampler, depth, true);
    }

    return Lo;
  }

} // namespace rt
