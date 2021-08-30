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

#ifndef IBSDF_H
#define IBSDF_H

#include <memory>

#include "rt/Sampler/Sample.h"

namespace tinyxml2 {
  class XMLElement;
} // namespace tinyxml2

namespace pt {

  struct IntersectionInfo;

  using BSDFPtr = std::unique_ptr<class IBSDF>;

  class IBSDF {
  public:
    IBSDF() noexcept;
    virtual ~IBSDF() noexcept;

    rt::Color color() const;
    void setColor(const rt::Color& c);

    /*
     * NOTE:
     *
     * wo := Direction of reflected radiance; AKA viewing direction.
     * wi := Direction of incident radiance.
     */

    // Convenience Functions in World Coordinates! ///////////////////////////

    rt::Color   eval(const IntersectionInfo& info, const rt::Direction& wi) const;
    rt::real_t   pdf(const IntersectionInfo& info, const rt::Direction& wi) const;
    rt::Color sample(const IntersectionInfo& info, rt::Direction *wi, const rt::Sample2D& xi) const;

    // Implementation in Shading Coordinates! ////////////////////////////////

    // Evaluate the BxDF: f(wo,wi)
    virtual rt::Color   eval(const rt::Direction& wo, const rt::Direction& wi) const = 0;
    // BxDF's Probability Density Function (PDF): pdf(wo,wi); usually just pdf(wi)
    virtual rt::real_t   pdf(const rt::Direction& wo, const rt::Direction& wi) const = 0;
    /*
     * Sample the BSDF and evaluate the Monte Carlo Rendering Equation w/o incident radiance:
     *
     * Returns: eval(wo,wi)*cos(ThetaI)/pdf(wo,wi)
     */
    virtual rt::Color sample(const rt::Direction& wo, rt::Direction *wi, const rt::Sample2D& xi) const = 0;

    static bool isBSDF(const tinyxml2::XMLElement *elem);
    static BSDFPtr load(const tinyxml2::XMLElement *elem);

  protected:
    rt::Color _color;

  private:
    static rt::Color readColor(const tinyxml2::XMLElement *parent);
  };

} // namespace pt

#endif // IBSDF_H
