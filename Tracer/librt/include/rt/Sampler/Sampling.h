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

#ifndef SAMPLING_H
#define SAMPLING_H

#include "rt/Sampler/Sample.h"

namespace rt {

  namespace sampling {

    size_t choose(const real_t xi, const size_t count);

    real_t powerHeuristic(const size_t nF, const real_t pdfF,
                          const size_t nG, const real_t pdfG);

  } // namespace sampling

  struct ConcentricDisk {
    static Vertex sample(const Sample2D& xi);
    static real_t pdf();
  };

  struct CosineHemisphere {
    static Direction sample(const Sample2D& xi);
    static real_t pdf(const real_t cosTheta);
  };

  struct UniformCone {
    static std::tuple<real_t,real_t> parameters(const Sample2D& xi, const real_t cosThetaMax);
    static Direction sample(const Sample2D& xi, const real_t cosThetaMax);
    static real_t pdf(const real_t cosThetaMax);
  };

  struct UniformDisk {
    static Vertex sample(const Sample2D& xi);
    static real_t pdf();
  };

  struct UniformHemisphere {
    static Direction sample(const Sample2D& xi);
    static real_t pdf();
  };

  struct UniformSphere {
    static Direction sample(const Sample2D& xi);
    static real_t pdf();
  };

} // namespace rt

#endif // SAMPLING_H
