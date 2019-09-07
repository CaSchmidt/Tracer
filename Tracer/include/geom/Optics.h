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

#ifndef OPTICS_H
#define OPTICS_H

#include "geom/Util.h"
#include "geom/Vec3.h"
#include "math/Pythagoras.h"

namespace geom {

  template<typename T>
  inline T fresnelReflectance(const Vec3<T>& I, const Vec3<T>& N,
                              const T& etai, const T& etat)
  {
    constexpr T ONE = static_cast<T>(1);
    constexpr T TWO = static_cast<T>(2);

    const T cosTi = -dot(I, N);
    const T sinTi = math::pythagoras(cosTi);

    const T sinTt = etai/etat*sinTi; // Snell's law
    if( sinTt >= ONE ) { // total internal reflection
      return ONE;
    }

    const T cosTt = math::pythagoras(sinTt);

    const T para = (etat*cosTi - etai*cosTt)/(etat*cosTi + etai*cosTt);
    const T perp = (etai*cosTi - etat*cosTt)/(etai*cosTi + etat*cosTt);

    return (para*para + perp*perp)/TWO;
  }

  template<typename T> // cf. GLSL
  constexpr Vec3<T> reflect(const Vec3<T>& I, const Vec3<T>& N)
  {
    return I - static_cast<T>(2)*dot(I, N)*N;
  }

  template<typename T> // cf. GLSL
  inline Vec3<T> refract(const Vec3<T>& I, const Vec3<T>& N, const T& eta)
  {
    constexpr T  ONE = static_cast<T>(1);
    constexpr T ZERO = static_cast<T>(0);

    const T DOT = dot(I, N);

    const T k = ONE - eta*eta*(ONE - DOT*DOT);
    if( k < ZERO ) {
      return Vec3<T>::zero();
    }

    return eta*I - (eta*DOT + std::sqrt(k))*N;
  }

} // namespace geom

#endif // OPTICS_H
