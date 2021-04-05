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

#ifndef IOBJECT_H
#define IOBJECT_H

#include <list>
#include <memory>

#include "rt/Material/IMaterial.h"

namespace rt {

  class IAreaLight;
  struct SurfaceInfo;

  class IObject {
  public:
    IObject(const Transform& objectToWorld) noexcept;
    virtual ~IObject() noexcept;

    virtual bool castShadow(const Ray& ray) const;

    // NOTE: All arguments passed to/returned from this method are in WORLD coordinates!
    virtual bool intersect(SurfaceInfo *surface, const Ray& ray) const = 0;

    IAreaLight *areaLight();
    const IAreaLight *areaLight() const;
    void setAreaLight(IAreaLight *light);

    IMaterial *material();
    const IMaterial *material() const;
    void setMaterial(MaterialPtr& material);

    void moveObject(const Transform& objectToWorld);
    const Transform& objectToWorld() const;
    void setObjectToWorld(const Transform& objectToWorld);

    template<typename T>
    inline T toObject(const T& x) const
    {
      return _xfrmOW*x;
    }

    template<typename T>
    inline T toWorld(const T& x) const
    {
      return _xfrmWO*x;
    }

    /*
     * Cf. to PBR3 Chapter "14.2.2 Sampling Shapes"
     * for an explanation of the following functions.
     */
    virtual real_t area() const = 0;
    virtual SurfaceInfo sample(const Sample2D& xi, real_t *pdf) const = 0;
    virtual real_t pdf(const SurfaceInfo& surface) const;
    // NOTE: The following functions compute the PDF with respect to solid angle!
    virtual SurfaceInfo sample(const SurfaceInfo& ref, const Sample2D& xi, real_t *pdf) const;
    virtual real_t pdf(const SurfaceInfo& ref, const Direction& wi) const;

  private:
    IObject() noexcept = delete;

    Transform   _xfrmWO{}; // Object -> World
    Transform   _xfrmOW{}; // World -> Object
    IAreaLight *_areaLight{nullptr};
    MaterialPtr _material{};
  };

  using ObjectPtr = std::unique_ptr<IObject>;
  using   Objects = std::list<ObjectPtr>;

  bool isDisk(const ObjectPtr& o);
  bool isPlane(const ObjectPtr& o);
  bool isSphere(const ObjectPtr& o);

} // namespace rt

#endif // IOBJECT_H
