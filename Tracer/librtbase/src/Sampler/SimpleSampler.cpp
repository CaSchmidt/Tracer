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

#include "rt/Sampler/SimpleSampler.h"

namespace rt {

  ////// public //////////////////////////////////////////////////////////////

  SimpleSampler::SimpleSampler(const size_t numSamplesPerPixel)
    : ISampler(numSamplesPerPixel)
  {
    _dis = std::uniform_real_distribution<real_t>(ZERO, ONE);

    std::random_device randDev;
    _gen.seed(randDev());
  }

  SimpleSampler::~SimpleSampler()
  {
  }

  SamplerPtr SimpleSampler::copy() const
  {
    return create(numSamplesPerPixel());
  }

  real_t SimpleSampler::sample() const
  {
    return _dis(*const_cast<std::mt19937*>(&_gen));
  }

  Sample2D SimpleSampler::sample2D() const
  {
    return Sample2D{sample(), sample()};
  }

  SamplerPtr SimpleSampler::create(const size_t numSamplesPerPixel)
  {
    return std::make_unique<SimpleSampler>(numSamplesPerPixel);
  }

} // namespace rt
