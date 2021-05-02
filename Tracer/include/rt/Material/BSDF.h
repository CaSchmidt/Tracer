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

#ifndef BSDF_H
#define BSDF_H

#include <array>

#include "rt/BxDF/IBxDF.h"
#include "rt/Texture/TexCoord.h"

namespace rt {

  class IMaterial;
  struct SurfaceInfo;

  class BSDF {
  public:
    BSDF(IMaterial *material) noexcept;
    ~BSDF() noexcept;

    bool add(const IBxDF *bxdf);
    size_t capacity() const;
    size_t count(const IBxDF::Flags flags = IBxDF::AllFlags) const;
    bool isEmpty() const;
    size_t size() const;

    const IBxDF *operator[](const size_t i) const;

    /*
     * Cf. to PBR3 Chapter "9.1 BSDFs" and Chapter "14.1.6 Sampling BSDFs"
     * for an explanation of the following functions.
     */
    Color eval(const SurfaceInfo& surface, const Direction& wi,
               const IBxDF::Flags flags = IBxDF::AllFlags) const;
    real_t pdf(const SurfaceInfo& surface, const Direction& wi,
               const IBxDF::Flags flags = IBxDF::AllFlags) const;
    Color sample(const SurfaceInfo& surface, Direction *wi, const Sample2D& xi, real_t *pdf,
                 const IBxDF::Flags flags = IBxDF::AllFlags,
                 IBxDF::Flags *sampledFlags = nullptr) const;

    template<typename T>
    inline T *asBxDF(const size_t i)
    {
      return dynamic_cast<T*>(const_cast<IBxDF*>(_bxdfs[i]));
    }

    template<typename T>
    inline const T *asBxDF(const size_t i) const
    {
      return dynamic_cast<const T*>(_bxdfs[i]);
    }

  private:
    BSDF() noexcept = delete;

    Color evalS(const Direction& wo, const Direction& wi, const TexCoord2D& tex,
                const IBxDF::Flags flags) const;
    real_t pdfS(const Direction& wo, const Direction& wi,
                const IBxDF::Flags flags) const;

    bool haveTexture(const size_t i) const;

    std::array<const IBxDF*,2> _bxdfs;
    IMaterial *_material{nullptr};
    size_t _numBxDFs{0};
  };

} // namespace rt

#endif // BSDF_H
