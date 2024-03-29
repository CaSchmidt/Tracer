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

#pragma once

#include "rt/Texture/TexCoord.h"

namespace rt {

  class IObject;

  struct SurfaceInfo {
    SurfaceInfo() noexcept = default;

    inline const IObject *operator->() const
    {
      return object;
    }

    void initializeShading(const Ray& ray);

    inline bool isHit() const
    {
      return geom::intersect::isHit(t)  &&  object != nullptr;
    }

    Color Le(const Direction& wo) const;

    inline Ray ray(const real_t tMax = Ray::MAX_T) const
    {
      return ray(wo, tMax);
    }

    inline Ray ray(const Direction& dir, const real_t tMax = Ray::MAX_T) const
    {
      return Ray(P, dir, geom::intersect::isHit(tMax)  &&  tMax < Ray::MAX_T
                 ? tMax - SHADOW_BIAS
                 : Ray::MAX_T);
    }

    Ray ray(const SurfaceInfo& to) const;

    inline TexCoord2D texCoord2D() const
    {
      return TexCoord2D{u, v};
    }

    template<typename VecT>
    inline VecT toShading(const VecT& v) const
    {
      return xfrmSW*v;
    }

    template<typename VecT>
    inline VecT toWorld(const VecT& v) const
    {
      return xfrmWS*v;
    }

    // NOTE: All members are in world coordinates!
    real_t      t{geom::intersect::NO_INTERSECTION};
    Direction  wo{};
    Direction woS{}; // 'wo' in shading coordinates
    Vertex      P{};
    Normal      N{};
    real_t      u{}, v{};
    Matrix xfrmSW; // World-to-Shading
    Matrix xfrmWS; // Shading-to-World
    const IObject *object{nullptr};
  };

} // namespace rt
