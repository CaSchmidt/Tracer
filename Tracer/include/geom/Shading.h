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

#ifndef SHADING_H
#define SHADING_H

#include "geom/Geom.h"

namespace geom {

  namespace shading {

    inline constexpr real_t ZERO = 0;
    inline constexpr real_t  ONE = 1;

    template<typename VecT>
    inline real_t cosTheta(const VecT& w)
    {
      return std::clamp<real_t>(w.z, -1, 1);
    }

    template<typename VecT>
    inline real_t absCosTheta(const VecT& w)
    {
      return n4::abs(cosTheta(w));
    }

    template<typename VecT>
    inline real_t cos2Theta(const VecT& w)
    {
      return cosTheta(w)*cosTheta(w);
    }

    template<typename VecT>
    inline bool isSameHemisphere(const VecT& w)
    {
      return w.z >= ZERO;
    }

    inline Direction reflect(const Direction& wo)
    {
      return {-wo.x, -wo.y, wo.z};
    }

    inline Direction refract(const Direction& wo, const real_t etai, const real_t etat)
    {
      const bool entering = isSameHemisphere(wo);
      const real_t eta = entering
          ? etai/etat
          : etat/etai;
      const real_t nz = entering
          ? +ONE
          : -ONE;
      const real_t  cosTi = std::clamp<real_t>(wo.z*nz, -1, 1);
      const real_t sin2Ti = std::max<real_t>(0, ONE - cosTi*cosTi);
      const real_t sin2Tt = eta*eta*sin2Ti; // Snell's Law
      const real_t cos2Tt = ONE - sin2Tt;
      return cos2Tt >= ZERO // Handle Internal Reflection: sin2Tt > 1
          ? Direction{-eta*wo.x, -eta*wo.y, -eta*wo.z + (eta*cosTi - n4::sqrt(cos2Tt))*nz}
          : Direction();
    }

    template<typename VecT>
    inline real_t sin2Theta(const VecT& w)
    {
      return std::max<real_t>(0, ONE - cos2Theta(w));
    }

    template<typename VecT>
    inline real_t sinTheta(const VecT& w)
    {
      return n4::sqrt(sin2Theta(w));
    }

  } // namespace shading

} // namespace geom

#endif // SHADING_H
