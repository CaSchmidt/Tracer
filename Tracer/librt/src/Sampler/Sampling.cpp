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

#include "rt/Sampler/Sampling.h"

#include "math/Solver.h"

namespace rt {

  ////// Public //////////////////////////////////////////////////////////////

  namespace sampling {

    size_t choose(const real_t xi, const size_t count)
    {
      const size_t index = size_t(n4::floor(xi*real_t(count)));
      return std::min<size_t>(index, count - 1);
    }

    real_t powerHeuristic(const size_t nF, const real_t pdfF,
                          const size_t nG, const real_t pdfG)
    {
      const real_t f = real_t(nF)*pdfF;
      const real_t g = real_t(nG)*pdfG;
      return (f*f)/(f*f + g*g);
    }

  } // namespace sampling

  ////// ConcentricDisk //////////////////////////////////////////////////////

  Vertex ConcentricDisk::sample(const Sample2D& xi)
  {
    SAMPLES_2D(xi);
    const Vertex offset = TWO*Vertex{xi1, xi2} - Vertex{1, 1};
    if( offset.isZero() ) {
      return Vertex();
    }
    const bool align_x = n4::abs(offset.x) > n4::abs(offset.y);
    const real_t     r = align_x
        ? real_t{offset.x}
        : real_t{offset.y};
    const real_t phi = align_x
        ? (PI_QUARTER)*(offset.y/offset.x)
        : (PI_HALF) - (PI_QUARTER)*(offset.x/offset.y);
    return {r*n4::cos(phi), r*n4::sin(phi)};
  }

  real_t ConcentricDisk::pdf()
  {
    return ONE/PI;
  }

  ////// CosineHemisphere ////////////////////////////////////////////////////

  Direction CosineHemisphere::sample(const Sample2D& xi)
  {
    const Vertex d = ConcentricDisk::sample(xi);
    const real_t z = n4::sqrt(std::max<real_t>(0, ONE - d.x*d.x - d.y*d.y));
    return {d.x, d.y, z};
  }

  real_t CosineHemisphere::pdf(const real_t cosTheta)
  {
    return cosTheta/PI;
  }

  ////// UniformCone /////////////////////////////////////////////////////////

  std::tuple<real_t,real_t> UniformCone::parameters(const Sample2D& xi, const real_t cosThetaMax)
  {
    SAMPLES_2D(xi);
    const real_t cosTheta = (ONE - xi1) + xi1*cosThetaMax;
    const real_t      phi = TWO_PI*xi2;
    return std::tuple<real_t,real_t>{cosTheta, phi};
  }

  Direction UniformCone::sample(const Sample2D& xi, const real_t cosThetaMax)
  {
    const auto [cosTheta, phi] = parameters(xi, cosThetaMax);
    const real_t sinTheta = math::pythagoras<real_t>(cosTheta);
    return geom::spherical(sinTheta, cosTheta, phi);
  }

  real_t UniformCone::pdf(const real_t cosThetaMax)
  {
    return ONE/TWO_PI/(ONE - cosThetaMax);
  }

  ////// UniformDisk /////////////////////////////////////////////////////////

  Vertex UniformDisk::sample(const Sample2D& xi)
  {
    SAMPLES_2D(xi);
    const real_t   r = n4::sqrt(xi1);
    const real_t phi = TWO_PI*xi2;
    return {r*n4::cos(phi), r*n4::sin(phi)};
  }

  real_t UniformDisk::pdf()
  {
    return ONE/PI;
  }

  ////// UniformHemisphere ///////////////////////////////////////////////////

  Direction UniformHemisphere::sample(const Sample2D& xi)
  {
    SAMPLES_2D(xi);
    const real_t cosTheta = xi1;
    const real_t sinTheta = math::pythagoras<real_t>(cosTheta);
    const real_t      phi = TWO_PI*xi2;
    return geom::spherical(sinTheta, cosTheta, phi);
  }

  real_t UniformHemisphere::pdf()
  {
    return ONE/TWO_PI;
  }

  ////// UniformSphere ///////////////////////////////////////////////////////

  Direction UniformSphere::sample(const Sample2D& xi)
  {
    SAMPLES_2D(xi);
    const real_t cosTheta = ONE - TWO*xi1;
    const real_t sinTheta = math::pythagoras<real_t>(cosTheta);
    const real_t      phi = TWO_PI*xi2;
    return geom::spherical(sinTheta, cosTheta, phi);
  }

  real_t UniformSphere::pdf()
  {
    return ONE/FOUR_PI;
  }

} // namespace rt
