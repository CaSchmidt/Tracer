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

#include "pt/BSDF/Mirror.h"

#include "geom/Shading.h"

namespace pt {

  Mirror::Mirror(const rt::real_t reflection) noexcept
  {
    setReflection(reflection);
  }

  Mirror::~Mirror() noexcept
  {
  }

  rt::Color Mirror::eval(const rt::Direction& /*wo*/, const rt::Direction& /*wi*/) const
  {
    return rt::Color(0);
  }

  rt::real_t Mirror::pdf(const rt::Direction& /*wo*/, const rt::Direction& /*wi*/) const
  {
    return 0;
  }

  rt::Color Mirror::sample(const rt::Direction& wo, rt::Direction *wi, const rt::Sample2D& /*xi*/) const
  {
    *wi = geom::shading::reflect(wo);
    return _color*_reflection;
  }

  rt::real_t Mirror::reflection() const
  {
    return _reflection;
  }

  void Mirror::setReflection(const rt::real_t r)
  {
    _reflection = std::clamp<rt::real_t>(r, 0, 1);
  }

  BSDFPtr Mirror::create(const rt::real_t reflection)
  {
    if( reflection < rt::ZERO  ||  reflection > rt::ONE ) {
      return BSDFPtr();
    }
    return std::make_unique<Mirror>(reflection);
  }

} // namespace pt
