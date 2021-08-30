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

#ifndef ITEXTURE_H
#define ITEXTURE_H

#include <memory>

#include "rt/Texture/TexCoord.h"

namespace tinyxml2 {
  class XMLElement;
} // namespace tinyxml2

namespace rt {

  using TexturePtr = std::unique_ptr<class ITexture>;

  class ITexture {
  public:
    ITexture(const size_t id) noexcept;
    virtual ~ITexture() noexcept;

    size_t id() const;

    virtual TexturePtr copy() const = 0;

    virtual Color lookup(const TexCoord2D& tex) const = 0;

    static bool isTexture(const tinyxml2::XMLElement *elem);
    static TexturePtr load(const tinyxml2::XMLElement *elem);

  protected:
    static size_t readId(const tinyxml2::XMLElement *elem, const rt::size_t defaultId = 0);

  private:
    ITexture() noexcept = delete;

    size_t _id{0};
  };

} // namespace rt

#endif // ITEXTURE_H
