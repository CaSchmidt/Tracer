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

#include <algorithm>

#include "rt/Material/BSDF.h"

#include "geom/Shading.h"
#include "rt/Material/IMaterial.h"
#include "rt/Object/SurfaceInfo.h"
#include "rt/Sampler/Sampling.h"

namespace rt {

  ////// public //////////////////////////////////////////////////////////////

  BSDF::BSDF(IMaterial *material) noexcept
    : _material{material}
  {
    _bxdfs.fill(nullptr);
  }

  BSDF::~BSDF() noexcept
  {
    for(size_t i = 0; i < size(); i++) {
      delete _bxdfs[i];
      _bxdfs[i] = nullptr;
    }
    _numBxDFs = 0;
  }

  bool BSDF::add(const IBxDF *bxdf)
  {
    if( bxdf == nullptr  ||  size() >= capacity() ) {
      return false;
    }
    _bxdfs[_numBxDFs++] = bxdf;
    return true;
  }

  size_t BSDF::capacity() const
  {
    return _bxdfs.size();
  }

  size_t BSDF::count(const IBxDF::Flags flags) const
  {
    return size_t(std::count_if(_bxdfs.begin(), _bxdfs.end(), [=](const IBxDF *bxdf) -> bool {
      return bxdf != nullptr  &&  bxdf->matchFlags(flags);
    }));
  }

  bool BSDF::isEmpty() const
  {
    return size() < 1;
  }

  size_t BSDF::size() const
  {
    return _numBxDFs;
  }

  const IBxDF *BSDF::operator[](const size_t i) const
  {
    return _bxdfs[i];
  }

  Color BSDF::eval(const SurfaceInfo& surface, const Direction& wi,
                   const IBxDF::Flags flags) const
  {
    const Direction wiS = surface.toShading(wi);
    return evalS(surface.woS, wiS, surface.texCoord2D(), flags);
  }

  real_t BSDF::pdf(const SurfaceInfo& surface, const Direction& wi,
                   const IBxDF::Flags flags) const
  {
    const Direction wiS = surface.toShading(wi);
    return pdfS(surface.woS, wiS, flags);
  }

  Color BSDF::sample(const SurfaceInfo& surface, Direction *wi, const Sample2D& xi, real_t *pdf,
                     const IBxDF::Flags flags, IBxDF::Flags *sampledFlags) const
  {
    if( pdf != nullptr ) {
      *pdf = 0;
    }
    if( sampledFlags != nullptr ) {
      *sampledFlags = IBxDF::InvalidFlags;
    }

    // (1) Choose Which BxDF to Sample ///////////////////////////////////////

    const size_t matching = count(flags);
    if( matching < 1 ) {
      *wi = Direction();
      return Color();
    }

    SAMPLES_2D(xi);

    const size_t choice = sampling::choose(xi1, matching);

    const IBxDF *bxdf = nullptr;
    for(size_t i = 0, count = choice; i < size(); i++) {
      if( _bxdfs[i]->matchFlags(flags)  &&  count-- == 0 ) {
        bxdf = _bxdfs[i];
        break;
      }
    }

    // (2) Remap BxDF Sample xi to [0,1)^2 ///////////////////////////////////

    const Sample2D xiRemapped{xi1*real_t(matching) - real_t(choice), xi2};

    // (3) Sample Chosen BxDF ////////////////////////////////////////////////

    Direction wiS;
    Color f = bxdf->sample(surface.woS, &wiS, xiRemapped, pdf);
    *wi = surface.toWorld(wiS);
    if( pdf != nullptr  &&  *pdf <= ZERO ) {
      return Color();
    }
    if( sampledFlags != nullptr ) {
      *sampledFlags = bxdf->flags();
    }

    // (4) Compute Overall PDF With All Matching BxDFs ///////////////////////

    if( pdf != nullptr ) {
      if( !bxdf->isSpecular()  &&  matching > 1 ) {
        *pdf = pdfS(surface.woS, wiS, flags);
      }
    }

    // (5) Compute Value of BSDF for Sampled Direction ///////////////////////

    if( !bxdf->isSpecular()  &&  matching > 1 ) {
      f = evalS(surface.woS, wiS, surface.texCoord2D(), flags);
    }

    return f;
  }

  ////// private /////////////////////////////////////////////////////////////

  Color BSDF::evalS(const Direction& wo, const Direction& wi, const TexCoord2D& tex,
                    const IBxDF::Flags flags) const
  {
    const bool is_reflect = geom::shading::isSameHemisphere(wo, wi);
    Color f;
    for(size_t i = 0; i < size(); i++) {
      const IBxDF *bxdf = _bxdfs[i];
      if( bxdf->matchFlags(flags)  &&
          ( ( is_reflect  &&  bxdf->isReflection()  )  ||
            (!is_reflect  &&  bxdf->isTransmission()) ) ) {
        f += haveTexture(i)
            ? bxdf->eval(wo, wi)*_material->textureLookup(i, tex)
            : bxdf->eval(wo, wi);
      }
    }
    return f;
  }

  real_t BSDF::pdfS(const Direction& wo, const Direction& wi,
                    const IBxDF::Flags flags) const
  {
    if( isEmpty()  ||  geom::shading::cosTheta(wo) == ZERO ) {
      return 0;
    }
    size_t matching = 0;
    real_t      pdf = 0;
    for(size_t i = 0; i < size(); i++) {
      const IBxDF *bxdf = _bxdfs[i];
      if( !bxdf->matchFlags(flags) ) {
        continue;
      }
      matching++;
      pdf += bxdf->pdf(wo, wi);
    }
    return matching > 0
        ? pdf/real_t(matching)
        : 0;
  }

  bool BSDF::haveTexture(const size_t i) const
  {
    return _material != nullptr  &&  _material->haveTexture(i);
  }

} // namespace rt
