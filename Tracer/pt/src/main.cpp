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

#include <cstdio>
#include <cstdlib>

#include "math/Solver.h"
#include "pt/BSDF/Dielectric.h"
#include "pt/BSDF/Diffuse.h"
#include "pt/BSDF/Mirror.h"
#include "pt/Renderer/PathTracer.h"
#include "pt/Scene/Scene.h"
#include "pt/Shape/Plane.h"
#include "pt/Shape/Sphere.h"
#include "rt/Camera/FrustumCamera.h"
#include "rt/Renderer/RenderContext.h"
#include "rt/Sampler/SimpleSampler.h"
#include "rt/Texture/FlatTexture.h"
#include "Util/Worker.h"

#define PATH_TO_XML "../../Tracer/Tracer/pt-scenes/"

#define FILE_1 "cornell.xml"
#define FILE_2 "cornell-shapes.xml"
#define FILE_3 "cornell-spheres.xml"

constexpr rt::size_t  blockSize = 8;
constexpr rt::size_t numSamples = 32;

constexpr rt::size_t  width = 768;
constexpr rt::size_t height = 768;

int main(int /*argc*/, char ** /*argv*/)
{
  rt::RenderContext rc;

  // (1) Scene & Options /////////////////////////////////////////////////////

  rc.scene = pt::Scene::create();

  rt::RenderOptions options;
  if( !pt::Scene::load(pt::SCENE(rc.scene), &options, PATH_TO_XML FILE_1) ) {
    return EXIT_FAILURE;
  }
  options.gamma    = 2.2f; // Not in XML!
  options.maxDepth = 5;    // Not in XML!

  // (2) Renderer ////////////////////////////////////////////////////////////

  rc.renderer = pt::PathTracer::create(options);

  // (3) Camera //////////////////////////////////////////////////////////////

  rc.camera = rt::FrustumCamera::create(width, height, rc.renderer->options());

  // (4) Sampler /////////////////////////////////////////////////////////////

  rc.sampler = rt::SimpleSampler::create(numSamples);

  // Done! ///////////////////////////////////////////////////////////////////

  Worker worker;
  const Image image = worker.execute(rc, blockSize);
  image.saveAsPNG("pt-output.png");

  return EXIT_SUCCESS;
}
