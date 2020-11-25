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
#include "rt/Scene/SurfaceInfo.h"

namespace rt {

  ////// public //////////////////////////////////////////////////////////////

  Color3f Renderer::castCameraRay(const Rayf& ray) const
  {
    return castRay(_view*ray);
  }

  void Renderer::clear()
  {
    _options = RenderOptions();
    _view    = Transformf();
    _scene.clear();
  }

  bool Renderer::initialize(const RenderOptions& options)
  {
    _options = options;

    const Transformf xfrmCW{Transformf::rotateX(-PI_HALF)};

    // Transform camera setup from world to camera coordinates...
    const Vertex3f      eyeC = xfrmCW*_options.eye;
    const Vertex3f   lookAtC = xfrmCW*_options.lookAt;
    const Normal3f cameraUpC = xfrmCW.scaledRotation()*_options.cameraUp;

    /*
     * NOTE:
     * The view transform is comprised of two transforms:
     * 1. camera coordinates are aligned using a look-at transform in camera space
     * 2. camera space is transformed to world space
     */
    _view = xfrmCW.inverse()*Transformf::lookAt(eyeC, lookAtC, cameraUpC);

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

  Color3f Renderer::castRay(const Rayf& ray, const unsigned int depth) const
  {
    if( depth >= _options.maxDepth ) {
      return _options.backgroundColor;
    }

    SurfaceInfo sinfo;
    if( !_scene.trace(sinfo, ray) ) {
      return _options.backgroundColor;
    }

    Color3f color;
    if(        sinfo->material()->isOpaque() ) {
      color = shade(sinfo, -ray.direction());

    } else if( sinfo->material()->isMirror() ) {
      const Normal3f     R = geom::reflect(ray.direction(), sinfo.N);
      const Color3f rcolor = castRay({sinfo.P + to_vertex(TRACE_BIAS*sinfo.N), R}, depth + 1);
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
      color = kR*castRay({sinfo.P + to_vertex(TRACE_BIAS*N), R}, depth + 1);

      // (4) Transmittance ///////////////////////////////////////////////////

      if( kT > ZERO ) {
        const Normal3f T = geom::refract(I, N, eta);
        color += kT*castRay({sinfo.P - to_vertex(TRACE_BIAS*N), T}, depth + 1);
      }

    }

    return color;
  }

  Color3f Renderer::shade(const SurfaceInfo& sinfo, const Normal3f& v) const
  {
    Color3f result;

    const OpaqueMaterial *opaque = sinfo->material()->opaque();

    for(const LightSourcePtr& light : _scene.lights()) {
      const LightInfo linfo = light->info(sinfo.P);

      if( _scene.trace({sinfo.P + to_vertex(TRACE_BIAS*sinfo.N), linfo.l, linfo.r}) ) {
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

} // namespace rt
