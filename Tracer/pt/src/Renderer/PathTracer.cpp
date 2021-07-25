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
                                 const rt::SamplerPtr& sampler, const rt::uint_t depth) const
  {
    const rt::RenderOptions& options = IRenderer::options();
    const Scene               *scene = SCENE(_scene);

    if( depth >= options.maxDepth ) {
      return rt::Color(0);
    }

    IntersectionInfo info;
    if( !scene->intersect(&info, ray) ) {
      return scene->backgroundColor();
    }

    const rt::Color Le = info.emittance();
    const rt::Color  f = info.textureColor()/rt::PI; // BxDF

    const rt::Direction wiS = rt::CosineHemisphere::sample(sampler->sample2D());
    const rt::Direction  wi = info.toWorld(wiS);
    const rt::real_t  cosTi = std::max<rt::real_t>(0, geom::shading::cosTheta(wiS));
    const rt::real_t    pdf = rt::CosineHemisphere::pdf(cosTi);

    const rt::Color Li = radiance(info.ray(wi), _scene, sampler, depth + 1);

    return Le + f*Li*cosTi/pdf;
  }

} // namespace pt
