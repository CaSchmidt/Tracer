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

#include "geom/Geom.h"
#include "math/Solver.h"

/*
 * Optics API:
 *
 * The API models the transmission of light from an incident media 'i'
 * into a media 't'. Throughout this API the following conventions hold:
 *
 * I   := The incident vector following the direction of light.
 * N   := The normal vector of the bounding surface pointing from the media 't' to the media 'i'.
 * eta := The ratio of the indices of refraction; eta = etai/etat.
 *
 * NOTE 1: All input vectors are considered normal AKA having a length of 1!
 *
 * NOTE 2: Properly setting up the geometry and refractive index is beyond the scope of this API
 *         and needs to be done by the caller!
 */

namespace geom {

  inline real_t fresnel(const Direction& I, const Normal& N, const real_t eta)
  {
    const real_t cosTi = std::clamp<real_t>(-n4::dot(I, to_direction(N)), -1, 1);
    const real_t sinTi = math::pythagoras(cosTi);

    const real_t sinTt = eta*sinTi; // Snell's law
    if( sinTt >= math::ONE<real_t> ) { // total internal reflection
      return math::ONE<real_t>;
    }

    const real_t cosTt = math::pythagoras(sinTt);

    const real_t para = (cosTi - eta*cosTt)/(cosTi + eta*cosTt);
    const real_t perp = (eta*cosTi - cosTt)/(eta*cosTi + cosTt);

    return (para*para + perp*perp)*math::ONE_HALF<real_t>;
  }

  // cf. GLSL v4.60, 8.5. Geometric Functions
  inline Direction reflect(const Direction& I, const Normal& N)
  {
    const real_t DOT = n4::dot(I, to_direction(N));
    return I - math::TWO<real_t>*DOT*to_direction(N);
  }

  // cf. GLSL v4.60, 8.5. Geometric Functions
  inline Direction refract(const Direction& I, const Normal& N, const real_t eta)
  {
    const real_t DOT = n4::dot(I, to_direction(N));

    const real_t k = math::ONE<real_t> - eta*eta*(math::ONE<real_t> - DOT*DOT);
    if( k < math::ZERO<real_t> ) {
      return Direction();
    }

    return eta*I - (eta*DOT + std::sqrt(k))*to_direction(N);
  }

} // namespace geom

#endif // OPTICS_H
