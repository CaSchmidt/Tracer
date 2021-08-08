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

#include "pt/Renderer/PathTracer.h"

#include "pt/Scene/Scene.h"
#include "pt/Shape/IntersectionInfo.h"

#include "geom/Shading.h"
#include "rt/Sampler/Sampling.h"

namespace pt {

  ////// public //////////////////////////////////////////////////////////////

  PathTracer::PathTracer(const rt::RenderOptions& options) noexcept
    : rt::IRenderer(options)
  {
  }

  PathTracer::~PathTracer() noexcept
  {
  }

  rt::RendererPtr PathTracer::create(const rt::RenderOptions& options)
  {
    return std::make_unique<PathTracer>(options);
  }

  ////// private /////////////////////////////////////////////////////////////

  rt::Color PathTracer::radiance(const rt::Ray& ray, const rt::ScenePtr& _scene,
                                 const rt::SamplerPtr& sampler,
                                 const rt::uint_t depth, const rt::Color& throughput) const
  {
    constexpr rt::real_t RR_EPSILON0 = 0x1p-3 - 0x1p-5;

    const rt::RenderOptions& options = IRenderer::options();
    const Scene               *scene = SCENE(_scene);

    IntersectionInfo info;
    if( !scene->intersect(&info, ray) ) {
      return scene->backgroundColor();
    }

    const rt::Color Le = info.emittance();

    const bool stop_path = depth >= options.maxDepth;
    const rt::real_t pdfRR = stop_path
        ? std::clamp<rt::real_t>(throughput.max(), RR_EPSILON0, rt::ONE - RR_EPSILON0)
        : 1;
    if( stop_path  &&  sampler->sample() >= pdfRR ) {
      return Le;
    }

    rt::Direction wi;
    const rt::Color bsdf = info.textureColor()*info.sampleBSDF(&wi, sampler->sample2D());

    const rt::Color beta = throughput*bsdf/pdfRR;

    const rt::Color Li = !bsdf.isZero()
        ? radiance(info.ray(wi), _scene, sampler, depth + 1, beta)
        : rt::Color(0);

    return Le + bsdf*Li/pdfRR;
  }

} // namespace pt
