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

#ifndef ILIGHT_H
#define ILIGHT_H

#include <list>
#include <memory>

#include "rt/Sampler/Sample.h"

namespace rt {

  struct SurfaceInfo;

  class ILight {
  public:
    enum Type : uint_t {
      Invalid        = 0,
      Area           = 1,
      DeltaDirection = 2,
      DeltaPosition  = 3
    };

    ILight(const Type type, const Transform& lightToWorld) noexcept;
    virtual ~ILight() noexcept;

    bool isDeltaLight() const;

    size_t numSamples() const;
    void setNumSamples(const size_t numSamples);

    Type type() const;

    virtual real_t pdfLi(const SurfaceInfo& ref, const Direction& wi) const = 0;
    virtual Color sampleLi(const SurfaceInfo& ref, Direction *wi,
                           const Sample2D& xi, real_t *pdf, Ray *vis) const = 0;

    template<typename VecT>
    inline VecT toLight(const VecT& v) const
    {
      return _xfrmLW*v;
    }

    template<typename VecT>
    inline VecT toWorld(const VecT& v) const
    {
      return _xfrmWL*v;
    }

  private:
    ILight() noexcept = delete;

    size_t    _numSamples{1};
    Type      _type{Invalid};
    Transform _xfrmWL{}; // Light -> World
    Transform _xfrmLW{}; // World -> Light
  };

  using LightPtr = std::unique_ptr<ILight>;
  using   Lights = std::list<LightPtr>;

} // namespace rt

#endif // ILIGHT_H
