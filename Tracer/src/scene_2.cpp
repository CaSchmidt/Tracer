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

#include "scenes.h"

rt::RenderOptions options_scene_2(const rt::dim_T width, const rt::dim_T height)
{
  rt::RenderOptions opt;

  opt.width           = width;
  opt.height          = height;
  opt.fov_rad         = rt::PI/rt::real_T{3.0};
  opt.backgroundColor = rt::Color3f{0, static_cast<rt::real_T>(0.8), 1};
  opt.lookAt          = rt::Vertex3f{0, 0, 0};
  opt.eye             = rt::Vertex3f{0, -7.5, 4};
  opt.cameraUp        = rt::Normal3f::zAxis();

  return opt;
}

rt::Objects create_scene_2()
{
  rt::Objects objs;

  const rt::Color3f  cw = rt::Color3f::white();
  const rt::real_T cdim = 0.8;

  const rt::real_T a = 1.5;

  rt::MaterialPtr material;

  // Ground //////////////////////////////////////////////////////////////////

  material = rt::OpaqueMaterial::create();
  material->opaque()->setDiffuse(rt::CheckedTexture::create(cw, cdim*cw, 5, 5));
  objs.push_back(rt::Plane::create(rt::Transformf(), material, 10, 10));

  // Sphere #1 ///////////////////////////////////////////////////////////////

  material = rt::OpaqueMaterial::create();
  material->opaque()->setDiffuse(rt::CheckedTexture::create(cw, cdim*cw, 4, 2));
  objs.push_back(rt::Sphere::create(rt::Transformf::translate({a, a, 1}), material, 1));

  // Sphere #2 ///////////////////////////////////////////////////////////////

  material = rt::OpaqueMaterial::create();
  material->opaque()->setDiffuse(rt::CheckedTexture::create(cw, cdim*cw, 4, 2));
  objs.push_back(rt::Sphere::create(rt::Transformf::translate({-a, a, 1}), material, 1));

  // Sphere #3 ///////////////////////////////////////////////////////////////

  material = rt::OpaqueMaterial::create();
  material->opaque()->setDiffuse(rt::CheckedTexture::create(cw, cdim*cw, 4, 2));
  objs.push_back(rt::Sphere::create(rt::Transformf::translate({-a, -a, 1}), material, 1));

  // Sphere #4 ///////////////////////////////////////////////////////////////

  material = rt::OpaqueMaterial::create();
  material->opaque()->setDiffuse(rt::CheckedTexture::create(cw, cdim*cw, 4, 2));
  objs.push_back(rt::Sphere::create(rt::Transformf::translate({a, -a, 1}), material, 1));

  return objs;
}

void initialize_scene_2(rt::Renderer& renderer, const rt::dim_T width, const rt::dim_T height)
{
  // (1) Create Scene ////////////////////////////////////////////////////////

  rt::Objects objs = create_scene_2();

  // (2) Add Light ///////////////////////////////////////////////////////////

  rt::LightSourcePtr light =
      rt::PointLight::create(rt::Color3f::white()*rt::TEN, {0, 0, 2});
  renderer.addLight(light);

  // (3) Initialize Renderer /////////////////////////////////////////////////

  renderer.initialize(options_scene_2(width, height), objs);
}
