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

#include <N4/Optics.h>

#include "rt/Renderer.h"

#include "geom/Optics.h"
#include "geom/Shading.h"
#include "rt/Object/SurfaceInfo.h"

namespace rt {

  ////// public //////////////////////////////////////////////////////////////

  Color Renderer::castCameraRay(const Ray& ray) const
  {
    return castRay(_view*ray);
  }

  void Renderer::clear()
  {
    _options = RenderOptions();
    _view    = Transform();
    _scene.clear();
  }

  bool Renderer::initialize(const RenderOptions& options)
  {
    _options = options;

    const Transform xfrmCW{Transform::rotateZYXbyPI2(0, 0, -1)};

    // Transform camera setup from world to camera coordinates...
    const Vertex         eyeC = xfrmCW*_options.eye;
    const Vertex      lookAtC = xfrmCW*_options.lookAt;
    const Direction cameraUpC = xfrmCW*_options.cameraUp;

    /*
     * NOTE:
     * The view transform is comprised of two transforms:
     * 1. camera coordinates are aligned using a look-at transform in camera space
     * 2. camera space is transformed to world space
     */
    _view = xfrmCW.inverse()*Transform::lookAt(eyeC, lookAtC, cameraUpC);

    return true;
  }

  const RenderOptions& Renderer::options() const
  {
    return _options;
  }

  void Renderer::setScene(Scene& scene)
  {
    _scene = std::move(scene);
  }

  ////// private /////////////////////////////////////////////////////////////

  Color Renderer::castRay(const Ray &ray, const unsigned int depth) const
  {
    SurfaceInfo info;
    if( !_scene.trace(info, ray) ) {
      return _options.backgroundColor;
    }

    const BxDFpack bxdfs = info->material()->getBxDFs();
    const BxDFdata  data(ray, info, _options.globalRefraction);

    Color color;
    for(const LightSourcePtr& light : _scene.lights()) {
      Ray       vis;
      Direction wiW;
      const Color Li = light->sampleLi(info, wiW, vis);

      if( _scene.trace(vis) ) {
        continue;
      }

      const Direction   wiS = data.toShading(wiW);
      const real_t absCosTi = geom::shading::absCosTheta(wiS);
      if( absCosTi <= ZERO ) {
        continue;
      }

      Color fR;
      for(size_t i = 0; i < bxdfs.size(); i++) {
        if( bxdfs[i] == nullptr ) {
          continue;
        }
        fR += info->material()->haveTexture(i)
            ? bxdfs[i]->eval(data.wo, wiS)*info->material()->textureLookup(i, info.texCoord2D())
            : bxdfs[i]->eval(data.wo, wiS);
      }

      color += fR*Li*absCosTi;
    }

    if( depth + 1 < _options.maxDepth ) {
      color += specularReflectAndRefract(bxdfs, data, info, depth);
    }

    return color;
  }

  Color Renderer::specularReflectAndRefract(const BxDFpack& bxdfs, const BxDFdata& data,
                                            const SurfaceInfo& sinfo, const unsigned int depth) const
  {
    Color color;
    for(const IBxDF *bxdf : bxdfs) {
      constexpr IBxDF::Flags flags = IBxDF::Flags(IBxDF::Specular | IBxDF::Reflection | IBxDF::Transmission);
      if( bxdf == nullptr  ||  !bxdf->matchFlags(flags) ) {
        continue;
      }

      Direction  wiS;
      const Color fR = bxdf->sample(data, wiS);
      if( fR.isZero()  ||  n4::isZero(geom::shading::cosTheta(wiS)) ) {
        continue;
      }

      const bool is_same = geom::shading::isSameHemisphere(wiS);
      const real_t  bias = is_same
          ? +TRACE_BIAS
          : -TRACE_BIAS;

      const Direction   wiW = data.toWorld(wiS);
      const Color        Li = castRay(sinfo.ray(wiW, bias), depth + 1);
      const real_t absCosTi = geom::shading::absCosTheta(wiS);

      color += fR*Li*absCosTi;
    }
    return color;
  }

} // namespace rt
