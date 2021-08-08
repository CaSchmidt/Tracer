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
#include "pt/BSDF/Diffuse.h"
#include "pt/Renderer/PathTracer.h"
#include "pt/Scene/Scene.h"
#include "pt/Shape/Plane.h"
#include "rt/Camera/FrustumCamera.h"
#include "rt/Renderer/RenderContext.h"
#include "rt/Sampler/SimpleSampler.h"
#include "rt/Texture/FlatTexture.h"
#include "Util/Worker.h"

constexpr rt::size_t  blockSize = 8;
constexpr rt::size_t numSamples = 32;

constexpr rt::size_t  width = 768;
constexpr rt::size_t height = 768;

void build_scene(pt::Scene *scene)
{  
  const rt::Color black{0, 0, 0};
  const rt::Color white{1, 1, 1};
  const rt::Color  gray{0.75, 0.75, 0.75};
  const rt::Color   red{0.75, 0.25, 0.25};
  const rt::Color green{0.25, 0.75, 0.25};
  const rt::Color   sky{0, 0.8f, 1};

  pt::BSDFPtr       bsdf;
  rt::Matrix      matrix;
  pt::ShapePtr     shape;
  rt::TexturePtr texture;

  scene->setBackgroundColor(black);

  pt::ObjectPtr cornell = pt::Object::createInvertedBox(n4::identity(), 2, 2, 2);
  bsdf = pt::Diffuse::create();
  cornell->setBSDF(bsdf);
  texture = rt::FlatTexture::create(gray);
  cornell->setTexture(0, texture);
  texture = rt::FlatTexture::create(red); // Left
  cornell->setTexture(1, texture);
  texture = rt::FlatTexture::create(green); // Right
  cornell->setTexture(2, texture);
  texture = rt::FlatTexture::create(black); // Front
  cornell->setTexture(3, texture);
  scene->add(cornell);

  pt::ObjectPtr light = pt::Object::create(n4::translate(0, 0, 0.99f)*n4::rotateXbyPI2(2));
  shape = pt::Plane::create(n4::identity(), 0.5, 0.5);
  light->add(shape);
  bsdf = pt::Diffuse::create();
  light->setBSDF(bsdf);
  texture = rt::FlatTexture::create(black);
  light->setTexture(1, texture);
  light->setEmissiveColor(white);
  light->setEmissiveScale(3);
  scene->add(light);

  {
    const rt::real_t d1 = 0.5;
    const rt::real_t h1 = 1;
    const rt::real_t p1 = 0.375;
    matrix = n4::translate(-p1, p1, h1/2.0 - 1.0)*n4::rotateZ(rt::PI/6.0);
    pt::ObjectPtr box1 = pt::Object::createBox(matrix, d1, d1, h1);
    bsdf = pt::Diffuse::create();
    box1->setBSDF(bsdf);
    texture = rt::FlatTexture::create(gray);
    box1->setTexture(0, texture);
    scene->add(box1);
  }

  {
    const rt::real_t d2 = 0.5;
    const rt::real_t h2 = 0.5;
    const rt::real_t p2 = 0.375;
    matrix = n4::translate(p2, -p2, h2/2.0 - 1.0)*n4::rotateZ(-rt::PI/6.0);
    pt::ObjectPtr box2 = pt::Object::createBox(matrix, d2, d2, h2);
    bsdf = pt::Diffuse::create();
    box2->setBSDF(bsdf);
    texture = rt::FlatTexture::create(gray);
    box2->setTexture(0, texture);
    scene->add(box2);
  }
}

int main(int /*argc*/, char ** /*argv*/)
{
  rt::RenderContext rc;

  // (1) Scene ///////////////////////////////////////////////////////////////

  rc.scene = pt::Scene::create();
  build_scene(pt::SCENE(rc.scene));

  // (2.1) Render Options ////////////////////////////////////////////////////

  rt::RenderOptions options;
  options.eye      = rt::Vertex{0, -2.7f, 0};
  options.lookAt   = rt::Vertex{0, 0, 0};
  options.cameraUp = rt::Direction{0, 0, 1};
  options.fov_rad       = math::radian<rt::real_t>(60);
  options.worldToScreen = 2;
  options.gamma    = 2.2f;
  options.maxDepth = 3;

  // (2.2) Renderer //////////////////////////////////////////////////////////

  rc.renderer = pt::PathTracer::create(options);

  // (3) Camera //////////////////////////////////////////////////////////////

  rc.camera = rt::FrustumCamera::create(width, height, options);

  // (4) Sampler /////////////////////////////////////////////////////////////

  rc.sampler = rt::SimpleSampler::create(numSamples);

  // Done! ///////////////////////////////////////////////////////////////////

  Worker worker;
  const Image image = worker.execute(rc, blockSize);
  image.saveAsPNG("pt-output.png");

  return EXIT_SUCCESS;
}
