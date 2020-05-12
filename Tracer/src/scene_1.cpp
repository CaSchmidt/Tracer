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

rt::RenderOptions options_scene_1(const rt::dim_T width, const rt::dim_T height)
{
  rt::RenderOptions opt;

  opt.width           = width;
  opt.height          = height;
  opt.fov_rad         = rt::PI/rt::real_T{3.0};
  opt.backgroundColor = rt::Color3f{0, static_cast<rt::real_T>(0.8), 1};
  opt.lookAt          = rt::Vertex3f{0, 0, 0};
  opt.eye             = rt::Vertex3f{-4, -7.5, 2};
  opt.cameraUp        = geom::zAxis<rt::Normal3f>();

  return opt;
}

rt::Objects create_scene_1()
{
  rt::Objects objs;

  const rt::Color3f cr = rt::Colors<rt::real_T>::red();
  const rt::Color3f cg = rt::Colors<rt::real_T>::green();
  const rt::Color3f cb = rt::Colors<rt::real_T>::blue();

  const rt::real_T cdim(0.8);

  rt::MaterialPtr material;

  // Ground Plane ////////////////////////////////////////////////////////////

  material = rt::OpaqueMaterial::create();
  material->opaque()->setDiffuse(rt::CheckedTexture::create(cr, cdim*cr, 5, 5));
  objs.push_back(rt::Plane::create(rt::Transformf(), material, 10, 10));

  // Sphere #1 ///////////////////////////////////////////////////////////////

  material = rt::OpaqueMaterial::create();
  material->opaque()->setDiffuse(rt::CheckedTexture::create(cg, cdim*cg, 4, 2));
  objs.push_back(rt::Sphere::create(rt::Transformf::translate({0, 0, 1}), material, 1));

  // Sphere #2 ///////////////////////////////////////////////////////////////

  material = rt::OpaqueMaterial::create();
  material->opaque()->setDiffuse(rt::CheckedTexture::create(cb, cdim*cb, 4, 2));
  material->opaque()->setSpecular(rt::FlatTexture::create(rt::Colors<rt::real_T>::white()));
  material->opaque()->setShininess(64);
  objs.push_back(rt::Sphere::create(rt::Transformf::translate({1, -1, 1.5}), material, 1));

  return objs;
}

void initialize_scene_1(rt::Renderer& renderer, const rt::dim_T width, const rt::dim_T height)
{
  // (1) Create Scene ////////////////////////////////////////////////////////

  rt::Objects objs = create_scene_1();

  // (2) Add Light ///////////////////////////////////////////////////////////

  rt::LightSourcePtr light =
      rt::DirectionalLight::create(rt::Colors<rt::real_T>::white()*rt::THREE, {1, -1, 4});
  renderer.addLight(light);

  // (3) Initialize Renderer /////////////////////////////////////////////////

  renderer.initialize(options_scene_1(width, height), objs);
}
