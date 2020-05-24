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

#include "rt/Renderer.h"

#include "geom/Optics.h"
#include "rt/Material/MirrorMaterial.h"
#include "rt/Material/OpaqueMaterial.h"
#include "rt/Material/TransparentMaterial.h"

namespace rt {

  ////// public //////////////////////////////////////////////////////////////

  void Renderer::addLight(LightSourcePtr& light)
  {
    _lights.push_back(std::move(light));
  }

  void Renderer::clear()
  {
    _camera  = Camera();
    _options = RenderOptions();
    _xfrmWC  = Transformf();
    _scene.clear();
    _lights.clear();
  }

  bool Renderer::initialize(const RenderOptions& options, Objects& scene)
  {
    clear();

    const Transformf xfrmCW{Transformf::rotateX(-PI_HALF)};

    _options = options;

    _scene = std::move(scene);

    const Vertex3f      eye = xfrmCW*_options.eye;
    const Vertex3f   lookAt = xfrmCW*_options.lookAt;
    const Normal3f cameraUp = xfrmCW.scaledRotation()*_options.cameraUp;

    _camera = Camera(eye, lookAt, cameraUp,
                     _options.width, _options.height, _options.fov_rad);

    _xfrmWC = xfrmCW.inverse();

    return true;
  }

  const RenderOptions& Renderer::options() const
  {
    return _options;
  }

  bool Renderer::render(const dim_T y, uint8_t *row, const uint8_t samples) const
  {
    if( y >= _options.height  ||  row == nullptr ) {
      return false;
    }
    if( samples > 1 ) {
      for(dim_T x = 0; x < _options.width; x++) {
        Color3f result;
        for(uint8_t s = 0; s < samples; s++) {
          const Color3f color = castRay(_xfrmWC*_camera.ray(x, y, true));
          result += cs::clamp(color, ZERO, ONE);
        }
        result /= samples;
        *row++ = result.r;
        *row++ = result.g;
        *row++ = result.b;
        *row++ = 0xFF;
      }
    } else {
      for(dim_T x = 0; x < _options.width; x++) {
        const Color3f color = castRay(_xfrmWC*_camera.ray(x, y));
        *row++ = color.r;
        *row++ = color.g;
        *row++ = color.b;
        *row++ = 0xFF;
      }
    }
    return true;
  }

  ////// private /////////////////////////////////////////////////////////////

  Color3f Renderer::castRay(const Rayf& ray, const real_T tMax, const unsigned int depth) const
  {
    if( depth >= _options.maxDepth ) {
      return _options.backgroundColor;
    }

    SurfaceInfo sinfo;
    if( !trace(sinfo, ray, tMax, NO_SHADOW_RAY) ) {
      return _options.backgroundColor;
    }

    Color3f color;
    if(        sinfo->material()->isOpaque() ) {
      color = shade(sinfo, -ray.direction());

    } else if( sinfo->material()->isMirror() ) {
      const Normal3f     R = geom::reflect(ray.direction(), sinfo.N);
      const Color3f rcolor = castRay({sinfo.P + geom::to_vertex<real_T>(TRACE_BIAS*sinfo.N), R}, MAX_REAL_T, depth + 1);
      color = sinfo->material()->mirror()->reflectance()*rcolor;

    } else if( sinfo->material()->isTransparent() ) {
      // (1) Set up Snell's law //////////////////////////////////////////////

      const Normal3f I = ray.direction();
      const bool entering = cs::dot(I, sinfo.N) < ZERO;
      const Normal3f N = entering
          ? Normal3f(sinfo.N)
          : Normal3f(-sinfo.N);
      const real_T etai = entering
          ? _options.globalRefraction
          : sinfo->material()->transparent()->refraction();
      const real_T etat = entering
          ? sinfo->material()->transparent()->refraction()
          : _options.globalRefraction;
      const real_T eta = etai/etat;

      // (2) Fresnel Reflectance & Transmittance /////////////////////////////

      const real_T kR = geom::fresnel(I, N, eta);
      const real_T kT = ONE - kR;

      // (3) Reflectance /////////////////////////////////////////////////////

      const Normal3f R = geom::reflect(I, N);
      color = kR*castRay({sinfo.P + geom::to_vertex<real_T>(TRACE_BIAS*N), R}, MAX_REAL_T, depth + 1);

      // (4) Transmittance ///////////////////////////////////////////////////

      if( kT > ZERO ) {
        const Normal3f T = geom::refract(I, N, eta);
        color += kT*castRay({sinfo.P - geom::to_vertex<real_T>(TRACE_BIAS*N), T}, MAX_REAL_T, depth + 1);
      }

    }

    return color;
  }

  Color3f Renderer::shade(const SurfaceInfo& sinfo, const Normal3f& v) const
  {
    Color3f result;

    const OpaqueMaterial *opaque = sinfo->material()->opaque();

    for(const LightSourcePtr& light : _lights) {
      const LightInfo linfo = light->info(sinfo.P);

      SurfaceInfo dummy;
      if( trace(dummy, {sinfo.P + geom::to_vertex<real_T>(TRACE_BIAS*sinfo.N), linfo.l}, linfo.r, SHADOW_RAY) ) {
        continue; // Light is obscured by an object!
      }

      const real_T cosTi = cs::dot1(sinfo.N, linfo.l);
      if( cosTi <= 0 ) {
        continue;
      }

      // Lambertian/Diffuse contribution
      Color3f scolor = opaque->diffuse(sinfo.u, sinfo.v);

      // Specular contribution
      if( opaque->isSpecular() ) {
        const Normal3f h = cs::normalize(linfo.l + v);
        const real_T cosTh = cs::dot1(sinfo.N, h);
        scolor += opaque->specular(sinfo.u, sinfo.v)*csPow(cosTh, opaque->shininess());
      }

      // Account for light's irradiance
      result += (scolor%linfo.EL)*cosTi;
    }

    return result;
  }

  bool Renderer::trace(SurfaceInfo& result, const Rayf& ray, const real_T tMax, const bool isShadowRay) const
  {
    result = SurfaceInfo();
    for(const ObjectPtr& object : _scene) {
      if( isShadowRay  &&  !object->material()->isShadowCaster() ) {
        continue;
      }
      SurfaceInfo hit;
      if( !object->intersect(hit, ray)  ||  hit.t >= tMax ) {
        continue;
      }
      if( !result.isHit()  ||  hit.t < result.t ) {
        result = hit;
      }
    }
    return result.isHit();
  }

} // namespace rt
