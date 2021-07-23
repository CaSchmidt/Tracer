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
#include "pt/Renderer/PathTracer.h"
#include "pt/Scene/Scene.h"
#include "pt/Shape/Plane.h"
#include "rt/Camera/FrustumCamera.h"
#include "rt/Renderer/RenderContext.h"
#include "rt/Sampler/SimpleSampler.h"
#include "Util/Worker.h"

constexpr rt::size_t numSamples = 16;

constexpr rt::size_t  width = 768;
constexpr rt::size_t height = 768;

namespace pt {

  void createBox(Scene *scene,
                 const rt::real_t dimx, const rt::real_t dimy, const rt::real_t dimz)
  {
    if( scene == nullptr  ||
        dimx <= rt::ZERO  ||  dimy <= rt::ZERO  ||  dimz <= rt::ZERO ) {
      return;
    }

    ShapePtr shape;
    rt::Matrix X;

    // (X.1) Left ////////////////////////////////////////////////////////////

    const rt::real_t left = -dimx/rt::TWO;
    X = n4::translate(left, 0, 0)*n4::rotateYbyPI2(3);
    shape = Plane::create(X, dimz, dimy);
    scene->add(shape);

    // (X.2) Right ///////////////////////////////////////////////////////////

    const rt::real_t right = dimx/rt::TWO;
    X = n4::translate(right, 0, 0)*n4::rotateYbyPI2(1);
    shape = Plane::create(X, dimz, dimy);
    scene->add(shape);

    // (Y.3) Front ///////////////////////////////////////////////////////////

    const rt::real_t front = -dimy/rt::TWO;
    X = n4::translate(0, front, 0)*n4::rotateXbyPI2(1);
    shape = Plane::create(X, dimx, dimz);
    scene->add(shape);

    // (Y.4) Back ////////////////////////////////////////////////////////////

    const rt::real_t back = dimy/rt::TWO;
    X = n4::translate(0, back, 0)*n4::rotateXbyPI2(3);
    shape = Plane::create(X, dimx, dimz);
    scene->add(shape);

    // (Z.5) Bottom //////////////////////////////////////////////////////////

    const rt::real_t bottom = -dimz/rt::TWO;
    X = n4::translate(0, 0, bottom)*n4::rotateXbyPI2(2);
    shape = Plane::create(X, dimx, dimy);
    scene->add(shape);

    // (Z.6) Top /////////////////////////////////////////////////////////////

    const rt::real_t top = dimz/rt::TWO;
    X = n4::translate(0, 0, top);
    shape = Plane::create(X, dimx, dimy);
    scene->add(shape);
  }

  void createInvertedBox(Scene *scene,
                         const rt::real_t dimx, const rt::real_t dimy, const rt::real_t dimz)
  {
    if( scene == nullptr  ||
        dimx <= rt::ZERO  ||  dimy <= rt::ZERO  ||  dimz <= rt::ZERO ) {
      return;
    }

    ShapePtr shape;
    rt::Matrix X;

    // (X.1) Left ////////////////////////////////////////////////////////////

    const rt::real_t left = -dimx/rt::TWO;
    X = n4::translate(left, 0, 0)*n4::rotateYbyPI2(1);
    shape = Plane::create(X, dimz, dimy);
    scene->add(shape);

    // (X.2) Right ///////////////////////////////////////////////////////////

    const rt::real_t right = dimx/rt::TWO;
    X = n4::translate(right, 0, 0)*n4::rotateYbyPI2(3);
    shape = Plane::create(X, dimz, dimy);
    scene->add(shape);

    // (Y.3) Front ///////////////////////////////////////////////////////////

    const rt::real_t front = -dimy/rt::TWO;
    X = n4::translate(0, front, 0)*n4::rotateXbyPI2(3);
    shape = Plane::create(X, dimx, dimz);
    scene->add(shape);

    // (Y.4) Back ////////////////////////////////////////////////////////////

    const rt::real_t back = dimy/rt::TWO;
    X = n4::translate(0, back, 0)*n4::rotateXbyPI2(1);
    shape = Plane::create(X, dimx, dimz);
    scene->add(shape);

    // (Z.5) Bottom //////////////////////////////////////////////////////////

    const rt::real_t bottom = -dimz/rt::TWO;
    X = n4::translate(0, 0, bottom);
    shape = Plane::create(X, dimx, dimy);
    scene->add(shape);

    // (Z.6) Top /////////////////////////////////////////////////////////////

    const rt::real_t top = dimz/rt::TWO;
    X = n4::translate(0, 0, top)*n4::rotateXbyPI2(2);
    shape = Plane::create(X, dimx, dimy);
    scene->add(shape);
  }

} // namespace pt

int main(int /*argc*/, char ** /*argv*/)
{
  rt::RenderContext rc;

  // (1) Scene ///////////////////////////////////////////////////////////////

  rc.scene = pt::Scene::create();
  pt::Scene *scene = pt::SCENE(rc.scene);
  scene->setBackgroundColor(rt::Color{0, 0.8f, 1});
  pt::createInvertedBox(scene, 2, 2, 2);

  // (2.1) Render Options ////////////////////////////////////////////////////

  rt::RenderOptions options;
  options.eye      = rt::Vertex{0, -2.5, 0};
  options.lookAt   = rt::Vertex{0, 0, 0};
  options.cameraUp = rt::Direction{0, 0, 1};
  options.fov_rad       = math::radian<rt::real_t>(60);
  options.worldToScreen = 2;
  options.gamma    = 1;
  options.maxDepth = 5;

  // (2.2) Renderer //////////////////////////////////////////////////////////

  rc.renderer = pt::PathTracer::create(options);

  // (3) Camera //////////////////////////////////////////////////////////////

  rc.camera = rt::FrustumCamera::create(width, height, options);

  // (4) Sampler /////////////////////////////////////////////////////////////

  rc.sampler = rt::SimpleSampler::create(numSamples);

  // Done! ///////////////////////////////////////////////////////////////////

  Worker worker;
  const Image image = worker.execute(rc);
  image.saveAsPNG("pt-output.png");

  return EXIT_SUCCESS;
}
