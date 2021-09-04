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

#ifndef ISHAPE_H
#define ISHAPE_H

#include <list>
#include <memory>

#include "rt/Base/Types.h"

namespace tinyxml2 {
  class XMLElement;
} // namespace tinyxml2

namespace pt {

  struct IntersectionInfo;

  using ShapePtr = std::unique_ptr<class IShape>;

  class IShape {
  public:
    IShape(const rt::Transform& shapeToWorld) noexcept;
    virtual ~IShape() noexcept;

    // NOTE: All arguments passed to/returned from this method are in WORLD coordinates!
    virtual bool intersect(IntersectionInfo *info, const rt::Ray& ray) const = 0;

    void moveShape(const rt::Transform& shapeToWorld);
    void setShapeToWorld(const rt::Transform& shapeToWorld);

    virtual rt::Bounds shapeBounds() const = 0;
    inline rt::Bounds worldBounds() const
    {
      return toWorld(shapeBounds());
    }

    template<typename T>
    inline T toShape(const T& x) const
    {
      return _xfrmSW*x;
    }

    template<typename T>
    inline T toWorld(const T& x) const
    {
      return _xfrmWS*x;
    }

    static bool isShape(const tinyxml2::XMLElement *elem);
    static ShapePtr load(const tinyxml2::XMLElement *elem);

  private:
    IShape() noexcept = delete;

    rt::Transform _xfrmWS{}; // Shape -> World
    rt::Transform _xfrmSW{}; // World -> Shape
  };

} // namespace pt

#endif // ISHAPE_H
