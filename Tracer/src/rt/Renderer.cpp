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
#include "rt/BxDF/Shading.h"
#if 0
#include "rt/Material/MirrorMaterial.h"
#include "rt/Material/OpaqueMaterial.h"
#include "rt/Material/TransparentMaterial.h"
#endif

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

#if 1
  Color Renderer::castRay(const Ray &ray, const unsigned int depth) const
  {
    SurfaceInfo sinfo;
    if( !_scene.trace(sinfo, ray) ) {
      return _options.backgroundColor;
    }

    const BxDFpack bxdfs = sinfo->material()->getBxDFs();
    const BxDFdata  data(ray, sinfo, _options.globalRefraction);

    Color color;
    for(const LightSourcePtr& light : _scene.lights()) {
      const LightInfo linfo = light->info(sinfo.P);

      if( _scene.trace({sinfo.P + geom::to_vertex(TRACE_BIAS*sinfo.N), linfo.l, linfo.r}) ) {
        continue;
      }

      const Direction wi = data.toShading(linfo.l);
      const real_t cosTi = shading::cosTheta(wi);
      if( cosTi <= ZERO ) {
        continue;
      }

      Color fR;
      for(size_t i = 0; i < bxdfs.size(); i++) {
        if( bxdfs[i] == nullptr ) {
          continue;
        }
        fR += sinfo->material()->haveTexture(i)
            ? bxdfs[i]->eval(data.wo, wi)*sinfo->material()->textureLookup(i, sinfo.u, sinfo.v)
            : bxdfs[i]->eval(data.wo, wi);
      }

      color += fR*linfo.EL*cosTi;
    }

    if( depth + 1 < _options.maxDepth ) {
      color += specularReflectAndRefract(bxdfs, data, sinfo, depth);
    }

    return color;
  }
#else
  Color Renderer::castRay(const Ray& ray, const unsigned int depth) const
  {
    if( depth >= _options.maxDepth ) {
      return Color();
    }

    SurfaceInfo sinfo;
    if( !_scene.trace(sinfo, ray) ) {
      return _options.backgroundColor;
    }

    Color color;
    if(        sinfo->material()->isOpaque() ) {
      color = shade(sinfo, -ray.direction());

    } else if( sinfo->material()->isMirror() ) {
      const Direction  R = n4::optics::reflect(ray.direction(), sinfo.N);
      const Color rcolor = castRay(Ray(sinfo.P + geom::to_vertex(TRACE_BIAS*sinfo.N), R), depth + 1);
      color = sinfo->material()->mirror()->reflectance()*rcolor;

    } else if( sinfo->material()->isTransparent() ) {
      // (1) Set up Snell's law //////////////////////////////////////////////

      const Direction I = ray.direction();
      const bool entering = !geom::isSameHemisphere(I, sinfo.N);
      const Normal N = entering
          ? Normal(sinfo.N)
          : Normal(-sinfo.N);
      const real_t etai = entering
          ? _options.globalRefraction
          : sinfo->material()->transparent()->refraction();
      const real_t etat = entering
          ? sinfo->material()->transparent()->refraction()
          : _options.globalRefraction;
      const real_t eta = etai/etat;

      // (2) Fresnel Reflectance & Transmittance /////////////////////////////

      const real_t cosTi = -n4::dot(I, geom::to_direction(N));
      const real_t    kR = geom::optics::dielectric(cosTi, eta);
      const real_t    kT = ONE - kR;

      // (3) Reflectance /////////////////////////////////////////////////////

      const Direction R = n4::optics::reflect(I, N);
      color = kR*castRay(Ray(sinfo.P + geom::to_vertex(TRACE_BIAS*N), R), depth + 1);

      // (4) Transmittance ///////////////////////////////////////////////////

      if( kT > ZERO ) {
        const Direction T = n4::optics::refract(I, N, eta);
        color += kT*castRay(Ray(sinfo.P - geom::to_vertex(TRACE_BIAS*N), T), depth + 1);
      }

    }

    return color;
  }
#endif

  Color Renderer::specularReflectAndRefract(const BxDFpack& bxdfs, const BxDFdata& data,
                                            const SurfaceInfo& sinfo, const unsigned int depth) const
  {
    Color color;
    for(const IBxDF *bxdf : bxdfs) {
      constexpr IBxDF::Type reflection = IBxDF::Type(IBxDF::Specular | IBxDF::Reflection);
      if( bxdf == nullptr  ||  !bxdf->isType(reflection) ) {
        continue;
      }

      Direction wiS;
      const Color fR = bxdf->sample(data, wiS);
      if( fR.isZero()  ||  n4::isZero(shading::cosTheta(wiS)) ) {
        continue;
      }

      const real_t bias = shading::isSameHemisphere(wiS)
          ? TRACE_BIAS
          : -TRACE_BIAS;

      const Direction wiW = data.toWorld(wiS);
      const Color      Li = castRay({sinfo.P + geom::to_vertex(bias*sinfo.N), wiW}, depth + 1);
      const real_t  cosTi = shading::cosTheta(wiS);

      color += fR*Li*cosTi;
    }
    return color;
  }

#if 0
  Color Renderer::shade(const SurfaceInfo& sinfo, const Direction& v) const
  {
    Color result;

    const OpaqueMaterial *opaque = sinfo->material()->opaque();

    for(const LightSourcePtr& light : _scene.lights()) {
      const LightInfo linfo = light->info(sinfo.P);

      if( _scene.trace(Ray(sinfo.P + geom::to_vertex(TRACE_BIAS*sinfo.N), linfo.l, linfo.r)) ) {
        continue; // Light is obscured by an object!
      }

      const real_t cosTi = geom::dot1(sinfo.N, geom::to_normal(linfo.l));
      if( cosTi <= 0 ) {
        continue;
      }

      // Lambertian/Diffuse contribution
      Color scolor = opaque->diffuse(sinfo.u, sinfo.v);

      // Specular contribution
      if( opaque->isSpecular() ) {
        const Direction h = n4::normalize(linfo.l + v);
        const real_t cosTh = geom::dot1(sinfo.N, geom::to_normal(h));
        scolor += opaque->specular(sinfo.u, sinfo.v)*n4::pow(cosTh, opaque->shininess());
      }

      // Account for light's irradiance
      result += (scolor*linfo.EL)*cosTi;
    }

    return result;
  }
#endif

} // namespace rt
