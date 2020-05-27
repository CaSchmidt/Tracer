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

#ifndef SCENES_H
#define SCENES_H

#include <string>

#include "rt/Color.h"
#include "rt/Light/DirectionalLight.h"
#include "rt/Light/PointLight.h"
#include "rt/Material/MirrorMaterial.h"
#include "rt/Material/OpaqueMaterial.h"
#include "rt/Material/TransparentMaterial.h"
#include "rt/Refraction.h"
#include "rt/Renderer.h"
#include "rt/Object/Cylinder.h"
#include "rt/Object/Plane.h"
#include "rt/Object/Sphere.h"
#include "rt/Texture/CheckedTexture.h"
#include "rt/Texture/FlatTexture.h"

void initialize_scene_1(rt::Renderer& renderer, const rt::dim_T width, const rt::dim_T height);

void initialize_scene_2(rt::Renderer& renderer, const rt::dim_T width, const rt::dim_T height);

void initialize_scene_3(rt::Renderer& renderer, const rt::dim_T width, const rt::dim_T height);

void initialize_scene_4(rt::Renderer& renderer, const rt::dim_T width, const rt::dim_T height);

void initialize_scene_cylinder(rt::Renderer& renderer, const rt::dim_T width, const rt::dim_T height);

void initialize_scene_text(rt::Renderer& renderer, const rt::dim_T width, const rt::dim_T height,
                           const std::string& text, const rt::real_T radius,
                           const rt::real_T dh, const rt::real_T dv,
                           const rt::Transformf& transform = rt::Transformf());

#endif // SCENES_H
