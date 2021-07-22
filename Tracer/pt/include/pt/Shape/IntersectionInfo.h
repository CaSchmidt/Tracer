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

#ifndef INTERSECTIONINFO_H
#define INTERSECTIONINFO_H

#include "rt/Texture/TexCoord.h"

namespace pt {

  class IShape;

  struct IntersectionInfo {
    IntersectionInfo() noexcept = default;

    void initializeShading(const rt::Ray& ray);

    inline bool isHit() const
    {
      return geom::intersect::isHit(t)  &&  shape != nullptr;
    }

    inline rt::Ray ray(const rt::real_t tMax = rt::Ray::MAX_T) const
    {
      return ray(wo, tMax);
    }

    inline rt::Ray ray(const rt::Direction& dir, const rt::real_t tMax = rt::Ray::MAX_T) const
    {
      return rt::Ray(P, dir, geom::intersect::isHit(tMax)  &&  tMax < rt::Ray::MAX_T
                     ? tMax - rt::SHADOW_BIAS
                     : rt::Ray::MAX_T);
    }

    rt::Ray ray(const IntersectionInfo& to) const;

    inline rt::TexCoord2D texCoord2D() const
    {
      return rt::TexCoord2D{u, v};
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
    rt::real_t      t{geom::intersect::NO_INTERSECTION};
    rt::Direction  wo{};
    rt::Direction woS{}; // 'wo' in shading coordinates
    rt::Vertex      P{};
    rt::Normal      N{};
    rt::real_t      u{}, v{};
    rt::Matrix xfrmSW; // World-to-Shading
    rt::Matrix xfrmWS; // Shading-to-World
    const IShape *shape{nullptr};
  };

} // namespace pt

#endif // INTERSECTIONINFO_H
