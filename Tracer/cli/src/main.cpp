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

#include <cstdio>
#include <cstdlib>

#include "rt/Camera/FrustumCamera.h"
#include "rt/Camera/SimpleCamera.h"
#include "rt/Loader/SceneLoader.h"
#include "rt/Renderer/DirectLightingRenderer.h"
#include "rt/Renderer/PathTracingRenderer.h"
#include "rt/Renderer/WhittedRenderer.h"
#include "rt/Sampler/SimpleSampler.h"
#include "rt/Scene/Scene.h"

#include "Worker.h"

#define BASE_PATH       "../../Tracer/Tracer/scenes/"
#define FILE_1          BASE_PATH "scene_1.xml"
#define FILE_2          BASE_PATH "scene_2.xml"
#define FILE_3          BASE_PATH "scene_3.xml"
#define FILE_4          BASE_PATH "scene_4.xml"
#define FILE_AREALIGHT  BASE_PATH "scene_arealight.xml"
#define FILE_BOX        BASE_PATH "scene_box.xml"
#define FILE_OBJECTS    BASE_PATH "scene_objects.xml"
#define FILE_SPHERES    BASE_PATH "scene_spheres.xml"
#define FILE_TEXT       BASE_PATH "scene_text.xml"

constexpr rt::size_t numSamples = 64;

constexpr rt::size_t  width = 1000;
constexpr rt::size_t height = 1000;

int main(int /*argc*/, char ** /*argv*/)
{
  const char *filename = FILE_1;

  rt::RenderContext rc;

  rt::RenderOptions options;
  if( !rt::loadScene(&rc.scene, &options, filename) ) {
    return EXIT_FAILURE;
  }
  //rc.scene.setUseCastShadow(true);

  rc.renderer = rt::DirectLightingRenderer::create(options);
  //rc.renderer = rt::PathTracingRenderer::create(options);
  //rc.renderer = rt::WhittedRenderer::create(options);

#if 0
  {
    rt::RenderOptions opts = renderer.options();
    opts.aperture = 0.5;
    opts.focus    = n4::distance(renderer.options().eye, renderer.options().lookAt);
    renderer.setOptions(opts);

    printf("aperture = %.3f\n", opts.aperture);
    printf("   focus = %.3f\n", opts.focus);
    fflush(stdout);
  }
#endif

#if 1
  rc.camera = rt::FrustumCamera::create(width, height, rc.renderer->options());
#else
  rc.camera = rt::SimpleCamera::create(width, height, rc.renderer->options());
#endif

  rc.sampler = rt::SimpleSampler::create(numSamples);

  Worker worker;
  const Image image = worker.execute(rc);
  image.saveAsPNG("output.png");

  return EXIT_SUCCESS;
}
