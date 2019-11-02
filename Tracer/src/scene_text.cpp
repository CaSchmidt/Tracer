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

#include <algorithm>

#include <font8x8_basic.h>

#include "scenes.h"

rt::RenderOptions options_scene_text(const rt::dim_T width, const rt::dim_T height)
{
  rt::RenderOptions opt;

  opt.width           = width;
  opt.height          = height;
  opt.fov_rad         = rt::PI/rt::real_T{3.0};
  opt.backgroundColor = rt::Color3f{0, static_cast<rt::real_T>(0.8), 1};
  opt.lookAt          = rt::Vertex3f{0, 0, 0};
  opt.eye             = rt::Vertex3f{-2, 4, 7.5};
  opt.cameraUp        = rt::Normal3f{0, 1, 0};
  opt.worldToCamera   = rt::Transformf{rt::Matrix3f::rotateX(-rt::PI_HALF)};

  return opt;
}

rt::Objects create_scene_text(std::string text, const rt::real_T radius,
                              const rt::real_T dh, const rt::real_T dv,
                              const rt::Transformf& transform)
{
  using size_type = std::string::size_type;

  rt::Objects objs;

  // Text ////////////////////////////////////////////////////////////////////

  if( text.empty() ) {
    return objs;
  }

  std::for_each(text.begin(), text.end(), [](char& ch) -> void {
    if( ch < 0x20  ||  ch > 0x7E ) {
      ch = 0x20;
    }
  });

  const rt::Color3f  cb = rt::Color3f::blue();
  const rt::Color3f  cg = rt::Color3f::green();
  const rt::Color3f  cr = rt::Color3f::red();
  const rt::real_T cdim = 0.8;

  rt::MaterialPtr material;

  // Ground //////////////////////////////////////////////////////////////////

  material = rt::OpaqueMaterial::create();
  material->opaque()->setDiffuse(rt::CheckedTexture::create(cr, cdim*cr, 5, 5));
  objs.push_back(rt::Plane::create(rt::Transformf(), material, 10, 10));

  // Spheres /////////////////////////////////////////////////////////////////

  const rt::real_T  width = dh*static_cast<rt::real_T>(text.size()*8);
  const rt::real_T height = dv*static_cast<rt::real_T>(8);
  const rt::real_T     x0 = -width/rt::TWO;
  const rt::real_T     z0 = height/rt::TWO;

  for(size_type i = 0; i < text.size(); i++) {
    const size_type index = static_cast<size_type>(text[i]);

    for(size_type row = 0; row < 8; row++) {
      const uint8_t  bits = static_cast<uint8_t>(font8x8_basic[index][row]);
      const rt::real_T oz = z0 - dv*(row);

      rt::real_T ox = x0 + dh*(i*8);
      for(size_type col = 0; col < 8; col++, ox += dh) {
        if( (bits & (1 << col)) == 0 ) {
          continue;
        }

        material = rt::MirrorMaterial::create();
        material->mirror()->setReflectance(0.8);

        objs.push_back(rt::Sphere::create(transform*rt::Transformf::translate({ox, 0, oz}),
                                          material, radius));
      } // For Each Column
    } // For Each Row
  } // For Each Glyph

  return objs;
}

void initialize_scene_text(rt::Renderer& renderer, const rt::dim_T width, const rt::dim_T height,
                           const std::string& text, const rt::real_T radius,
                           const rt::real_T dh, const rt::real_T dv,
                           const rt::Transformf& transform)
{
  // (1) Create Scene ////////////////////////////////////////////////////////

  rt::Objects objs = create_scene_text(text, radius, dh, dv, transform);

  // (2) Add Light ///////////////////////////////////////////////////////////

  rt::LightSourcePtr light =
      rt::DirectionalLight::create(rt::Color3f::white()*rt::THREE, {-1, -1, 1});
  renderer.addLight(light);

  // (3) Initialize Renderer /////////////////////////////////////////////////

  renderer.initialize(options_scene_text(width, height), objs);
}
