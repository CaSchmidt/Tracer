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

#ifndef SURFACEINFO_H
#define SURFACEINFO_H

#include "rt/Texture/TexCoord.h"
#include "rt/Types.h"

namespace rt {

  class IObject;

  struct SurfaceInfo {
    SurfaceInfo() noexcept = default;

    inline const IObject *operator->() const
    {
      return object;
    }

    inline bool isHit() const
    {
      return geom::intersect::isHit(t)  &&  object != nullptr;
    }

    inline Vertex biasedP(const real_t bias = 0) const
    {
      return P + bias*geom::to_vertex(N);
    }

    Color Le(const Direction& wo) const;

    inline Ray ray(const real_t bias = 0, const real_t tMax = Ray::MAX_T) const
    {
      return ray(wo, bias, tMax);
    }

    inline Ray ray(const Direction& dir, const real_t bias = 0, const real_t tMax = Ray::MAX_T) const
    {
      return Ray{biasedP(bias), dir, tMax};
    }

    Ray ray(const SurfaceInfo& to, const real_t bias = 0) const;

    inline TexCoord2D texCoord2D() const
    {
      return TexCoord2D{u, v};
    }

    // NOTE: All members are in world coordinates!
    real_t     t{geom::intersect::NO_INTERSECTION};
    Direction wo{};
    Vertex     P{};
    Normal     N{};
    real_t     u{}, v{};
    const IObject *object{nullptr};
  };

} // namespace rt

#endif // SURFACEINFO_H
