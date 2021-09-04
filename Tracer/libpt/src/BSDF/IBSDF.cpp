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

#include "pt/BSDF/IBSDF.h"

#include "pt/Shape/IntersectionInfo.h"

namespace pt {

  IBSDF::IBSDF() noexcept
  {
    setColor(rt::Color(1));
  }

  IBSDF::~IBSDF() noexcept
  {
  }

  rt::Color IBSDF::color() const
  {
    return _color;
  }

  void IBSDF::setColor(const rt::Color& c)
  {
    _color = n4::clamp(c, 0, 1);
  }

  rt::Color IBSDF::eval(const IntersectionInfo& info, const rt::Direction& wi) const
  {
    return eval(info.woS, info.toShading(wi));
  }

  rt::real_t IBSDF::pdf(const IntersectionInfo& info, const rt::Direction& wi) const
  {
    return pdf(info.woS, info.toShading(wi));
  }

  rt::Color IBSDF::sample(const IntersectionInfo& info, rt::Direction *wi, const rt::Sample2D& xi) const
  {
    rt::Direction wiS;
    const rt::Color result = sample(info.woS, &wiS, xi);
    *wi = info.toWorld(wiS);
    return result;
  }

} // namespace pt
