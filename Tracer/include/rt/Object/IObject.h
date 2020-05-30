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
#include "rt/SurfaceInfo.h"

namespace rt {

  class IObject {
  public:
    IObject(const Transformf& objectToWorld, MaterialPtr& material) noexcept;
    virtual ~IObject() noexcept;

    // NOTE: All arguments passed to/returned from this method are in WORLD coordinates!
    virtual bool intersect(SurfaceInfo& info, const Rayf& ray) const = 0;

    IMaterial *material();
    const IMaterial *material() const;

  protected:
    Transformf  _xfrmWO{}; // Object -> World
    Transformf  _xfrmOW{}; // World -> Object
    MaterialPtr _material{};

  private:
    IObject() noexcept = delete;
  };

  using ObjectPtr = std::unique_ptr<IObject>;
  using   Objects = std::list<ObjectPtr>;

} // namespace rt

#endif // IOBJECT_H