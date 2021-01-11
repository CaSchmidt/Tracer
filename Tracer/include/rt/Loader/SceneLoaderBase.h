/****************************************************************************
** Copyright (c) 2020, Carsten Schmidt. All rights reserved.
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

#ifndef SCENELOADERBASE_H
#define SCENELOADERBASE_H

#include <charconv>
#include <string>
#include <type_traits>

#include <tinyxml2.h>

#include "rt/Types.h"

namespace rt {

  namespace priv {

    real_t parseAngle(const tinyxml2::XMLElement *node, bool *ok);

    Color parseColor(const tinyxml2::XMLElement *node, bool *ok, const bool clamp = true);

    Direction parseDirection(const tinyxml2::XMLElement *node, bool *ok);

    Normal parseNormal(const tinyxml2::XMLElement *node, bool *ok);

    real_t parseReal(const tinyxml2::XMLElement *node, bool *ok);

    Matrix parseRotation(const tinyxml2::XMLElement *node, bool *ok);

    size_t parseSize(const tinyxml2::XMLElement *node, bool *ok);

    std::string parseString(const tinyxml2::XMLElement *node, bool *ok);

    Transform parseTransform(const tinyxml2::XMLElement *node, bool *ok);

    Vertex parseVertex(const tinyxml2::XMLElement *node, bool *ok);

  } // namespace priv

} // namespace rt

#endif // SCENELOADERBASE_H
